// textbuffer.h
// Joel W. Matthys, 08/27/2012
// rtcmix~ functions for parsing score files
// formerly lines 701-831 of rtcmix~.c, now separated for easier dev

#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-maxmsp-4.0.1.6"

#include "textbuffer.h"

#include "ext.h"
#include "z_dsp.h"
#include "string.h"
#include "ext_strings.h"
#include "edit.h"
#include "ext_wind.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buffer.h"
#include "ext_obex.h"
#include <dlfcn.h>
int dylibincr;

#define MAX_INPUTS 100  //arbitrary -- for Damon!
#define MAX_OUTPUTS 20	//also arbitrary
#define MAX_SCRIPTS 20	//how many scripts can we store internally

// see the note for rtcmix_dotext() below
void rtcmix_text(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        if (x->flushflag == 1) return; // heap and queue being reset
        defer_low(x, (method)rtcmix_dotext, s, argc, argv); // always defer this message
}

// what to do when we get the message "text"
// rtcmix~ scores come from the [textedit] object this way
void rtcmix_dotext(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i, varnum;
        char thebuf[8192]; // should #define these probably
        char xfer[8192];
        char *bptr;
        int nchars;
        int top;

        bptr = thebuf;
        nchars = 0;
        top = 0;

        for (i=0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                sprintf(xfer, " %ld", argv[i].a_w.w_long);
                                break;
                        case A_FLOAT:
                                sprintf(xfer, " %lf", argv[i].a_w.w_float);
                                break;
                        case A_DOLLAR:
                                varnum = argv[i].a_w.w_long;
                                if ( !(x->var_set[varnum-1]) ) error("variable $%d has not been set yet, using 0.0 as default",varnum);
                                sprintf(xfer, " %lf", x->var_array[varnum-1]);
                                break;
                        case A_SYM:
                                if (top == 0) { sprintf(xfer, "%s", argv[i].a_w.w_sym->s_name); top = 1;}
                                else sprintf(xfer, " %s", argv[i].a_w.w_sym->s_name);
                                break;
                        case A_SEMI:
                                sprintf(xfer, ";");
                                break;
                        case A_COMMA:
                                sprintf(xfer, ",");
                }
                strcpy(bptr, xfer);
                nchars = strlen(xfer);
                bptr += nchars;
        }


        // ok, here's the deal -- when cycling tokenizes the message stream, if a quoted param (like a pathname to a soundfile)
        // contains any spaces it will be split into to separate, unquoted Symbols.  The function below repairs this for
        // certain specific rtcmix commands.  Not really elegant, but I don't know of another solution at present
        rtcmix_badquotes("rtinput", thebuf);
        rtcmix_badquotes("system", thebuf);
        rtcmix_badquotes("dataset", thebuf);

        if (x->parse_score(thebuf, strlen(thebuf)) != 0) error("problem parsing RTcmix script");
}


// see the note in rtcmix_dotext() about why I have to do this
void rtcmix_badquotes(char *cmd, char *thebuf)
{
        int i;
        char *rtinputptr;
        Boolean badquotes, checkon;
        int clen;
        char tbuf[8192];

        // jeez this just sucks big giant easter eggs
        badquotes = false;
        rtinputptr = strstr(thebuf, cmd); // find (if it exists) the instance of the command that may have a split in the quoted param
        if (rtinputptr) {
                rtinputptr += strlen(cmd);
                clen = strlen(thebuf) - (rtinputptr-thebuf);
                checkon = false;
                for (i = 0; i < clen; i++) { // start from the command and look for spaces, between ( )
                        if (*rtinputptr++ == '(' ) checkon = true;
                        if (checkon) {
                                if ((int)*rtinputptr == 34) { // we found a quote, so its cool and we can stop
                                        i = clen;
                                } else if (*rtinputptr == ')' ) {  // uh oh, no quotes and this command expects them
                                        badquotes = true;
                                        i = clen;
                                }
                        }
                }
        }

        // lordy, look at this code.  I wish cycling would come up with an unaltered buf-passing type
        if (badquotes) { // now we're gonna put in the missing quotes
                rtinputptr = strstr(thebuf, cmd);
                rtinputptr += strlen(cmd);
                checkon = false;
                for (i = 0; i < clen; i++) {
                        if (*rtinputptr++ == '(' ) checkon = true;
                                if (checkon)
                                        if (*rtinputptr != ' ') i = clen;
                } // at this point we're at the beginning of the should-be-quoted param in the buffer

                // so we copy it to a temporary buffer, insert a quote...
                strcpy(tbuf, rtinputptr);
                *rtinputptr++ = 34;
                strcpy(rtinputptr, tbuf);

                // and then put in the rest of the param, inserting a quote at the end of the should-be-quoted param
                clen = strlen(thebuf) - (rtinputptr-thebuf);
                for (i = 0; i < clen; i++)
                        if (*(++rtinputptr) == ')' ) {
                                while (*(--rtinputptr) == ' ') { }
                                rtinputptr++;
                                i = clen;
                        }

                // and this splices the modified, happily-quoted-param buffer back to the buf we give to rtcmix
                strcpy(tbuf, rtinputptr);
                *rtinputptr++ = 34;
                strcpy(rtinputptr, tbuf);
        }
}


// see the note for rtcmix_dortcmix() below
void rtcmix_rtcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        if (x->flushflag == 1) return; // heap and queue being reset
        defer_low(x, (method)rtcmix_dortcmix, s, argc, argv); // always defer this message
}


// what to do when we get the message "rtcmix"
// used for single-shot RTcmix scorefile commands
void rtcmix_dortcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i;
        double p[1024]; // should #define this probably
        char *cmd = NULL;

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                p[i-1] = (double)(argv[i].a_w.w_long);
                                break;
                        case A_FLOAT:
                                p[i-1] = (double)(argv[i].a_w.w_float);
                                break;
                        case A_SYM:
                                cmd = argv[i].a_w.w_sym->s_name;
                }
        }

        x->parse_dispatch(cmd, p, argc-1, NULL);
}


// the "var" message allows us to set $n variables imbedded in a scorefile with varnum value messages
void rtcmix_var(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i, varnum;

        for (i = 0; i < argc; i += 2) {
                varnum = argv[i].a_w.w_long;
                if ( (varnum < 1) || (varnum > NVARS) ) {
                        error("only vars $1 - $9 are allowed");
                        return;
                }
                x->var_set[varnum-1] = true;
                switch (argv[i+1].a_type) {
                        case A_LONG:
                                x->var_array[varnum-1] = (float)(argv[i+1].a_w.w_long);
                                break;
                        case A_FLOAT:
                                x->var_array[varnum-1] = argv[i+1].a_w.w_float;
                }
        }
}


// the "varlist" message allows us to set $n variables imbedded in a scorefile with a list of positional vars
void rtcmix_varlist(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i;

        if (argc > NVARS) {
                        error("asking for too many variables, only setting the first 9 ($1-$9)");
                        argc = NVARS;
        }

        for (i = 0; i < argc; i++) {
                x->var_set[i] = true;
                switch (argv[i].a_type) {
                        case A_LONG:
                                x->var_array[i] = (float)(argv[i].a_w.w_long);
                                break;
                        case A_FLOAT:
                                x->var_array[i] = argv[i].a_w.w_float;
                }
        }
}


// the "bufset" message allows access to a [buffer~] object.  The only argument is the name of the [buffer~]
void rtcmix_bufset(t_rtcmix *x, t_symbol *s)
{
        t_buffer *b;

        if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer) {
                x->buffer_set(s->s_name, b->b_samples, b->b_frames, b->b_nchans, b->b_modtime);
        } else {
                error("rtcmix~: no buffer~ %s", s->s_name);
        }
}

// the "flush" message
void rtcmix_flush(t_rtcmix *x)
{
        x->flushflag = 1; // set a flag, the flush will happen in perform after pulltraverse()
}


// here is the text-editor buffer stuff, go dan trueman go!
// used for rtcmix~ internal buffers
void rtcmix_edclose (t_rtcmix *x, char **text, long size)
{
        if (x->rtcmix_script[x->current_script]) {
                sysmem_freeptr((void *)x->rtcmix_script[x->current_script]);
                x->rtcmix_script[x->current_script] = 0;
        }
        x->rtcmix_script_len[x->current_script] = size;
    x->rtcmix_script[x->current_script] = (char *)sysmem_newptr((size+1) * sizeof(char)); // size+1 so we can add '\0' at end
        strncpy(x->rtcmix_script[x->current_script], *text, size);
        x->rtcmix_script[x->current_script][size] = '\0'; // add the terminating '\0'
        x->m_editor = NULL;
}


void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result)
{
        *result = 3; //don't put up dialog box
        return;
}


// open up an ed window on the current buffer
void rtcmix_dblclick(t_rtcmix *x)
{
        char title[80];

        if (x->m_editor) {
                if(x->rtcmix_script[x->current_script])
                        object_method(x->m_editor, gensym("settext"), x->rtcmix_script[x->current_script], gensym("utf-8"));
        } else {
                x->m_editor = object_new(CLASS_NOBOX, gensym("jed"), (t_object *)x, 0);
                sprintf(title,"script_%d", x->current_script);
                object_attr_setsym(x->m_editor, gensym("title"), gensym(title));
                if(x->rtcmix_script[x->current_script])
                        object_method(x->m_editor, gensym("settext"), x->rtcmix_script[x->current_script], gensym("utf-8"));
        }

        object_attr_setchar(x->m_editor, gensym("visible"), 1);
}


// see the note for rtcmix_goscript() below
void rtcmix_goscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        if (x->flushflag == 1) return; // heap and queue being reset
        defer_low(x, (method)rtcmix_dogoscript, s, argc, argv); // always defer this message
}


// the [goscript N] message will cause buffer N to be sent to the RTcmix parser
void rtcmix_dogoscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i,j,temp = 0;
        int tval;
        int buflen;
        #define MAXSCRIPTSIZE 16384
        char thebuf[MAXSCRIPTSIZE]; // should probably by dyn-alloced, or at least set to coordinate with RTcmix (if necessary)

        if (argc == 0) {
                error("rtcmix~: goscript needs a buffer number [0-19]");
                return;
        }

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                break;
                        case A_FLOAT:
                                temp = (short)argv[i].a_w.w_float;
                }
        }

        if (temp > MAX_SCRIPTS) {
                error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                temp = MAX_SCRIPTS-1;
        }
        if (temp < 0) {
                error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                temp = 0;
        }
        x->current_script = temp;

        if (x->rtcmix_script_len[x->current_script] == 0)
                post("rtcmix~: you are triggering a 0-length script!");

        buflen = x->rtcmix_script_len[x->current_script];
        if (buflen >= MAXSCRIPTSIZE) {
                error("rtcmix~ script %d is too large!", x->current_script);
                return;
        }

        // probably don't need to transfer to a new buffer, but I want to be sure there's room for the \0,
        // plus the substitution of \n for those annoying ^M thingies
        for (i = 0, j = 0; i < buflen; i++) {
                thebuf[j] = *(x->rtcmix_script[x->current_script]+i);
                if ((int)thebuf[j] == 13) thebuf[j] = '\n'; // RTcmix wants newlines, not <cr>'s

                // ok, here's where we substitute the $vars
                if (thebuf[j] == '$') {
                        sscanf(x->rtcmix_script[x->current_script]+i+1, "%d", &tval);
                        if ( !(x->var_set[tval-1]) ) error("variable $%d has not been set yet, using 0.0 as default", tval);
                        sprintf(thebuf+j, "%f", x->var_array[tval-1]);
                        j = strlen(thebuf)-1;
                        i++; // skip over the var number in input
                }
                j++;
        }
        thebuf[j] = '\0';

        // I need to find out why this crashes under 10.3.x, but has to wait until I have a 10.3.x machine
        if ( (sys_getdspstate() == 1) || (strncmp(thebuf, "system", 6) == 0) ) { // don't send if the dacs aren't turned on, unless it is a system() <------- HACK HACK HACK!
                if (x->parse_score(thebuf, j) != 0) error("problem parsing RTcmix script");
        }
}


// [openscript N] will open a buffer N
void rtcmix_openscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i,temp = 0;

        if (argc == 0) {
                error("rtcmix~: openscript needs a buffer number [0-19]");
                return;
        }

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                break;
                        case A_FLOAT:
                                temp = (short)argv[i].a_w.w_float;
                }
        }

        if (temp > MAX_SCRIPTS) {
                error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                temp = MAX_SCRIPTS-1;
        }
        if (temp < 0) {
                error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                temp = 0;
        }

        x->current_script = temp;
        rtcmix_dblclick(x);
}


// [setscript N] will set the currently active script to N
void rtcmix_setscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i,temp = 0;

        if (argc == 0) {
                error("rtcmix~: setscript needs a buffer number [0-19]");
                return;
        }

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                break;
                        case A_FLOAT:
                                temp = (short)argv[i].a_w.w_float;
                }
        }

        if (temp > MAX_SCRIPTS) {
                error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                temp = MAX_SCRIPTS-1;
        }
        if (temp < 0) {
                error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                temp = 0;
        }

        x->current_script = temp;
}


// the [savescript] message triggers this
void rtcmix_write(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i, temp = 0;

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                break;
                        case A_FLOAT:
                                temp = (short)argv[i].a_w.w_float;
                }
        }

        if (temp > MAX_SCRIPTS) {
                error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                temp = MAX_SCRIPTS-1;
        }
        if (temp < 0) {
                error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                temp = 0;
        }

        x->current_script = temp;
        post("rtcmix: current script is %d", temp);

        defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
}


// the [savescriptas] message triggers this
void rtcmix_writeas(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        short i, temp = 0;

        for (i=0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                if (temp > MAX_SCRIPTS) {
                                        error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                                        temp = MAX_SCRIPTS-1;
                                }
                                if (temp < 0) {
                                        error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                                        temp = 0;
                                }
                                x->current_script = temp;
                                x->s_name[x->current_script][0] = 0;
                                break;
                        case A_FLOAT:
                                temp = (short)argv[i].a_w.w_float;
                                if (temp > MAX_SCRIPTS) {
                                        error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                                        temp = MAX_SCRIPTS-1;
                                }
                                if (temp < 0) {
                                        error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                                        temp = 0;
                                }
                                x->current_script = temp;
                                x->s_name[x->current_script][0] = 0;
                                break;
                        case A_SYM://this doesn't work yet
                                strcpy(x->s_name[x->current_script], argv[i].a_w.w_sym->s_name);
                                post("rtcmix~: writing file %s",x->s_name[x->current_script]);
                }
        }
        post("rtcmix: current script is %d", temp);

        defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
}


// deferred from the [save*] messages
void rtcmix_dowrite(t_rtcmix *x, Symbol *s, short argc, t_atom *argv)
{
        char filename[256];
        t_handle script_handle;
        short err;
        long type_chosen, thistype = 'TEXT';
        t_filehandle fh;

        if(!x->s_name[x->current_script][0]) {
                //if (saveas_dialog(&x->s_name[0][x->current_script], &x->path[x->current_script], &type))
                  if (saveasdialog_extended(x->s_name[x->current_script], &x->path[x->current_script], &type_chosen, &thistype, 1))
                        return; //user cancelled
        }
        strcpy(filename, x->s_name[x->current_script]);

        err = path_createsysfile(filename, x->path[x->current_script], thistype, &fh);
        if (err) {
                fh = 0;
                error("rtcmix~: error %d creating file", err);
                return;
        }

        script_handle = sysmem_newhandle(0);
        sysmem_ptrandhand (x->rtcmix_script[x->current_script], script_handle, x->rtcmix_script_len[x->current_script]);

        err = sysfile_writetextfile(fh, script_handle, TEXT_LB_UNIX);
        if (err) {
                fh = 0;
                error("rtcmix~: error %d writing file", err);
                return;
        }

        // BGG for some reason mach-o doesn't like this one... the memory hit should be small
//	sysmem_freehandle(script_handle);
        sysfile_seteof(fh, x->rtcmix_script_len[x->current_script]);
        sysfile_close(fh);

        return;

}

// the [read ...] message triggers this
void rtcmix_read(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        defer(x, (method)rtcmix_doread, s, argc, argv); // always defer this message
}

// the deferred read
void rtcmix_doread(t_rtcmix *x, Symbol *s, short argc, t_atom *argv)
{
        char filename[256];
        short err, i, temp = 0;
        long type = 'TEXT';
        long size;
        long outtype;
        t_filehandle fh;
        t_handle script_handle;

        for (i = 0; i < argc; i++) {
                switch (argv[i].a_type) {
                        case A_LONG:
                                temp = (short)argv[i].a_w.w_long;
                                if (temp > MAX_SCRIPTS) {
                                        error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                                        temp = MAX_SCRIPTS-1;
                                }
                                if (temp < 0) {
                                        error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                                        temp = 0;
                                }
                                x->current_script = temp;
                                x->s_name[x->current_script][0] = 0;
                                break;
                        case A_FLOAT:
                                if (temp > MAX_SCRIPTS) {
                                        error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
                                        temp = MAX_SCRIPTS-1;
                                }
                                if (temp < 0) {
                                        error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
                                        temp = 0;
                                }
                                x->current_script = temp;
                                temp = (short)argv[i].a_w.w_float;
                                x->s_name[x->current_script][0] = 0;
                                break;
                        case A_SYM:
                                strcpy(filename, argv[i].a_w.w_sym->s_name);
                                strcpy(x->s_name[x->current_script], filename);
                }
        }


        if(!x->s_name[x->current_script][0]) {
//		if (open_dialog(filename, &path, &outtype, &type, 1))
                if (open_dialog(filename,  &x->path[x->current_script], &outtype, 0L, 0)) // allow all types of files

                        return; //user cancelled
        } else {
                if (locatefile_extended(filename, &x->path[x->current_script], &outtype, &type, 1)) {
                        error("rtcmix~: error opening file: can't find file");
                        x->s_name[x->current_script][0] = 0;
                        return; //not found
                }
        }

        //we should have a valid filename at this point
        err = path_opensysfile(filename, x->path[x->current_script], &fh, READ_PERM);
        if (err) {
                fh = 0;
                error("error %d opening file", err);
                return;
        }

        strcpy(x->s_name[x->current_script], filename);

        sysfile_geteof(fh, &size);
        if (x->rtcmix_script[x->current_script]) {
                sysmem_freeptr((void *)x->rtcmix_script[x->current_script]);
                x->rtcmix_script[x->current_script] = 0;
        }
        // BGG size+1 in max5 to add the terminating '\0'
        if (!(x->rtcmix_script[x->current_script] = (char *)sysmem_newptr(size+1)) || !(script_handle = sysmem_newhandle(size+1))) {
                error("rtcmix~: %s too big to read", filename);
                return;
        } else {
                x->rtcmix_script_len[x->current_script] = size;
                sysfile_readtextfile(fh, script_handle, size, TEXT_LB_NATIVE);
                strcpy(x->rtcmix_script[x->current_script], *script_handle);
        }
        x->rtcmix_script[x->current_script][size] = '\0'; // the max5 text editor apparently needs this
        // BGG for some reason mach-o doesn't like this one... the memory hit should be small
//	sysmem_freehandle(*script_handle);
        sysfile_close(fh);

        return;
}


// this converts the current script to a binbuf format and sets it up to be saved and then restored
// via the rtcmix_restore() method below
void rtcmix_save(t_rtcmix *x, void *w)
{
        char *fptr, *tptr;
        char tbuf[5000]; // max 5's limit on symbol size is 32k, this is totally arbitrary on my part
//	char *tbuf;
        int i,j,k;


        // insert the command to recreate the rtcmix~ object, with any additional vars
        binbuf_vinsert(w, "ssll", gensym("#N"), gensym("rtcmix~"), x->num_inputs, x->num_pinlets);

        for (i = 0; i < MAX_SCRIPTS; i++) {
                if (x->rtcmix_script[i] && (x->rtcmix_script_len[i] > 0)) { // there is a script...
                        // the reason I do this 'chunking' of restore messages is because of the 32k limit
                        // I still wish Cycling had a generic, *untouched* buffer type.
                        fptr = x->rtcmix_script[i];
                        tptr = tbuf;
                        k = 0;
                        for (j = 0; j < x->rtcmix_script_len[i]; j++) {
                                *tptr++ = *fptr++;
                                if (++k >= 5000) { // 'serialize' the script
                                        // the 'restore' message contains script #, current buffer length, final buffer length, symbol with buffer contents
                                        *tptr = '\0';
                                        binbuf_vinsert(w, "ssllls", gensym("#X"), gensym("restore"), i, k, x->rtcmix_script_len[i], gensym(tbuf));
                                        tptr = tbuf;
                                        k = 0;
                                }
                        }
                        // do the final one (or the only one in cases where scripts < 5000)
                        *tptr = '\0';
                        binbuf_vinsert(w, "ssllls", gensym("#X"), gensym("restore"), i, k, x->rtcmix_script_len[i], gensym(tbuf));
                }
        }
}


// and this gets the message set up by rtcmix_save()
void rtcmix_restore(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
        int i;
        int bsize, fsize;
        char *fptr; // restore buf pointer is in the struct for repeated calls necessary for larger scripts (symbol size limit, see rtcmix_save())

        // script #, current buffer size, final script size, script data
        x->current_script = argv[0].a_w.w_long;
        bsize = argv[1].a_w.w_long;
        if (argv[2].a_type == A_SYM) { // this is for v1.399 scripts that had the symbol size limit bug
                fsize = bsize;
                fptr = argv[2].a_w.w_sym->s_name;
        } else {
                fsize = argv[2].a_w.w_long;
                fptr = argv[3].a_w.w_sym->s_name;
        }

        if (!x->rtcmix_script[x->current_script]) { // if the script isn't being restored already
                if (!(x->rtcmix_script[x->current_script] = (char *)sysmem_newptr(fsize+1))) { // fsize+1 for the '\0
                        error("rtcmix~: problem allocating memory for restored script");
                        return;
                }
                x->rtcmix_script_len[x->current_script] = fsize;
                x->restore_buf_ptr = x->rtcmix_script[x->current_script];
        }

        // this happy little for-loop is for older (max 4.x) rtcmix scripts.  The older version of max had some
        // serious parsing issues for saved text.  Now it all seems fixed in 5 -- yay!
        // convert the xRTCMIX_XXx tokens to their real equivalents
        for (i = 0; i < bsize; i++) {
                switch (*fptr) {
                        case 'x':
                                if (strncmp(fptr, "xRTCMIX_CRx", 11) == 0) {
                                        sprintf(x->restore_buf_ptr, "\r");
                                        fptr += 11;
                                        x->restore_buf_ptr++;
                                        break;
                                }
                                else if (strncmp(fptr, "xRTCMIX_DQx", 11) == 0) {
                                        sprintf(x->restore_buf_ptr, "\"");
                                        fptr += 11;
                                        x->restore_buf_ptr++;
                                        break;
                                } else {
                                        *x->restore_buf_ptr++ = *fptr++;
                                        break;
                                }
                        default:
                                *x->restore_buf_ptr++ = *fptr++;
                }
        }

        x->rtcmix_script[x->current_script][fsize] = '\0'; // the final '\0'

        x->current_script = 0; // do this to set script 0 as default
}
