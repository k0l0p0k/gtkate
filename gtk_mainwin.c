#include "gtk_mainwin.h"

void mainwin_init (mainwin_t *mwin, char **argv)
{
    mwin->window = NULL;
    mwin->toolbar = NULL;
    mwin->vboxtree = NULL;
    mwin->view = NULL;

    mwin->winwidth = 780;
    mwin->winheight = 800;

    mwin->showtoolbar = TRUE;
    mwin->showdocwin = TRUE;

    if (argv) {}
}

void mainwin_destroy (mainwin_t *mwin)
{
    if (mwin) {};
}

/*
 * window callbacks
 */
/** on_window_delete_event called first when WM_CLOSE button clicked.
 *  if return is FALSE, on_window_destroy is called, if TRUE, control
 *  is returned to gtk_main and you are responsible for gtk_main_quit.
 */
/** on_window_destroy called after on_window_delete_event processed
 *  if the return from on_window_delete_event is FALSE.
 */
void on_window_destroy (GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
    if (widget) {}
    if (data) {}
}

gboolean on_window_delete_event (GtkWidget *widget, GdkEvent *event,
                                 gpointer data)
{
    on_window_destroy (widget, data);
    return FALSE;

    if (widget) {}
    if (event) {}
}

/** create application window & initialize values
 *  and connect callback functions. 'app' contains
 *  widgets for window, text_view and statusbar.
 */
GtkWidget *create_window (mainwin_t *mwin)
{
    GtkAccelGroup *mainaccel;
    GtkWidget *vbox;                /* vbox container   */
    GtkWidget *menubar;             /* menu container   */
//     GtkWidget *hbox;                /* hbox container   */
    GtkWidget *hpaned;             /* hpaned widget */
    GtkWidget *vboxibscroll;        /* vbox for infobar & scrolled_window */
    GtkWidget *ibarvbox;            /* infobar vbox container   */
    GtkWidget *scrolled_textview;   /* scrolled win w/text_view */
//     GtkWidget *textview;            /* textview */
    GtkWidget *statusbar;           /* statusbar */
//     GtkWidget *label;

    GtkWidget *sbalign;             /* alignment for statusbar  */
    guint ptop;                     /* padding, top, bot, l, r  */
    guint pbot;
    guint pleft;
    guint pright;

    GtkWidget *fileMenu;            /* file menu        */
    GtkWidget *fileMi;
    GtkWidget *sep;
    GtkWidget *quitMi;
    GtkWidget *showtbMi;
    GtkWidget *showdocMi;

    GtkToolItem *exit;              /* toolbar */

    GtkTreeSelection *selection;    /* document tree selection for callback */
    gint treewidth = 180;

    /* temp vars */
    PangoFontDescription *font_desc;    /* font description */
    gint bordersz = 0;

    // gchar *iconfile;            /* filename to loading icon */

    /* create toplevel window */
    if (!(mwin->window = gtk_window_new (GTK_WINDOW_TOPLEVEL))) {
        g_fprintf (stderr, "create_window() gtk_window_new failure.\n");
        return NULL;
    }
    // gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_position (GTK_WINDOW (mwin->window), GTK_WIN_POS_NONE);
    gtk_window_set_default_size (GTK_WINDOW (mwin->window), mwin->winwidth,
                                    mwin->winheight);
    // // gtk_window_move (GTK_WINDOW (app->window), app->winrootx, app->winrooty);

    /* create icon filename and set icon */
    /*
    if ((iconfile = g_strdup_printf ("%s/%s", app->imgdir, ICON))) {
        gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf_from_file (iconfile));
        g_free (iconfile);
    }
    */

    /* create & attach accelerator group */
    mainaccel = gtk_accel_group_new ();
    gtk_window_add_accel_group (GTK_WINDOW (mwin->window), mainaccel);

    /* create vbox to hold menu, toolbar, scrolled_windows, textview & statusbar
     * and add contaier to main window
     */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (mwin->window), vbox);
    // gtk_box_set_spacing (GTK_BOX (vbox), 0);

    /* create menubar and menus to add */
    // menubar = create_menubar (app, mainaccel);
    // app->menubar = menubar;
    menubar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);
    gtk_widget_show (menubar);

    fileMenu = gtk_menu_new ();
    fileMi = gtk_menu_item_new_with_mnemonic ("_File");
    sep = gtk_separator_menu_item_new ();
    quitMi   = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT,
                                                   NULL);
    showtbMi = gtk_image_menu_item_new_from_stock (GTK_STOCK_CONVERT,
                                                   NULL);
    gtk_menu_item_set_label (GTK_MENU_ITEM (showtbMi), "_Show/Hide Toolbar");
    showdocMi = gtk_image_menu_item_new_from_stock (GTK_STOCK_DIRECTORY,
                                                   NULL);
    gtk_menu_item_set_label (GTK_MENU_ITEM (showdocMi), "Show/Hide _Documents");
    /* create entries under 'File' then add to menubar */
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (fileMi), fileMenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), sep);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), quitMi);
    sep = gtk_separator_menu_item_new ();
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), sep);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), showtbMi);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), showdocMi);
    gtk_menu_shell_append (GTK_MENU_SHELL (menubar), fileMi);
    gtk_widget_add_accelerator (quitMi, "activate", mainaccel,
                                GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator (showtbMi, "activate", mainaccel,
                                GDK_KEY_t, GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator (showdocMi, "activate", mainaccel,
                                GDK_KEY_d, GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);

    /* create toolbar
     * GTK_TOOLBAR_ICONS, GTK_TOOLBAR_TEXT, GTK_TOOLBAR_BOTH, GTK_TOOLBAR_BOTH_HORIZ
     */
    // app->toolbar = create_toolbar (mainaccel, app);
    mwin->toolbar = gtk_toolbar_new ();
    gtk_box_pack_start(GTK_BOX(vbox), mwin->toolbar, FALSE, FALSE, 0);
    gtk_widget_show (mwin->toolbar);

    gtk_container_set_border_width(GTK_CONTAINER(mwin->toolbar), 2);
    gtk_toolbar_set_show_arrow (GTK_TOOLBAR(mwin->toolbar), TRUE);
    gtk_toolbar_set_style(GTK_TOOLBAR(mwin->toolbar), GTK_TOOLBAR_ICONS);

    exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_tool_item_set_homogeneous (exit, FALSE);
    gtk_toolbar_insert(GTK_TOOLBAR(mwin->toolbar), exit, -1);
    gtk_widget_set_tooltip_text (GTK_WIDGET(exit), "Quit ");

    /* create hbox to display dirtree and ibar/scrolled_window */
//     hbox = gtk_hbox_new (FALSE, 0);
    hpaned = gtk_hpaned_new();
    gtk_paned_set_position (GTK_PANED(hpaned), treewidth);
    gtk_container_set_border_width(GTK_CONTAINER(hpaned), 2);

    // gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hpaned, TRUE, TRUE, 0);
    // gtk_container_add (GTK_CONTAINER (vbox), hbox);
    // gtk_box_set_spacing (GTK_BOX (hbox), 5);
//     gtk_widget_show (hbox);
    gtk_widget_show (hpaned);
    /* create vbox for directory tree */
    mwin->vboxtree = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_size_request (mwin->vboxtree, treewidth, -1);
//     gtk_box_pack_start(GTK_BOX(hbox), mwin->vboxtree, FALSE, TRUE, 0);
    gtk_paned_pack1 (GTK_PANED(hpaned), mwin->vboxtree, TRUE, FALSE);
    gtk_widget_show (mwin->vboxtree);
//     label = gtk_label_new ("\nvboxtree region");
//     gtk_box_pack_start(GTK_BOX(mwin->vboxtree), label, FALSE, FALSE, 0);
//     gtk_widget_show (label);

    /* create scrolled_window for tree */
    GtkWidget *treescroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (treescroll),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_widget_show (treescroll);

    mwin->doctreeview = create_view_and_model();
    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(mwin->doctreeview));
    /*
     * border
     */
    gtk_container_set_border_width(GTK_CONTAINER(mwin->vboxtree), bordersz);
    gtk_container_add (GTK_CONTAINER (treescroll), mwin->doctreeview);
    gtk_box_pack_start(GTK_BOX(mwin->vboxtree), treescroll, TRUE, TRUE, 0);
//     gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(mwin->doctreeview), FALSE);
    gtk_widget_show (mwin->doctreeview);

    /* create vbox for infobar and scrolled_window */
    vboxibscroll = gtk_vbox_new (FALSE, 0);
    // gtk_container_add (GTK_CONTAINER (hbox), vboxibscroll);
//     gtk_box_pack_end(GTK_BOX(hbox), vboxibscroll, TRUE, TRUE, 0);
    gtk_paned_pack2 (GTK_PANED(hpaned), vboxibscroll, TRUE, TRUE);
    gtk_widget_show (vboxibscroll);

    /* create vbox to display infobar */
    ibarvbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vboxibscroll), ibarvbox, FALSE, FALSE, 0);
    gtk_widget_show (ibarvbox);

    /* create scrolled_window and textview */
    mwin->view = gtk_text_view_new ();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (mwin->view), GTK_WRAP_WORD);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW (mwin->view), 5);
    gtk_widget_show (mwin->view);
    scrolled_textview = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_textview),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled_textview), mwin->view);
    /*
     * border
     */
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_textview), bordersz);
    gtk_box_pack_start (GTK_BOX (vboxibscroll), scrolled_textview, TRUE, TRUE, 0);
    gtk_widget_show (scrolled_textview);

    /* Change default font throughout the widget */
    font_desc = pango_font_description_from_string ("DejaVu Sans Mono 8");
    gtk_widget_modify_font (mwin->view, font_desc);

    /* set tab to lesser of softtab and tabstop if softtab set */
    set_tab_size (font_desc, mwin->view, 4);
    pango_font_description_free (font_desc);

    /* create/pack statusbar at end within gtk_alignment */
    sbalign = gtk_alignment_new (0, .5, 1, 1);
    gtk_alignment_get_padding (GTK_ALIGNMENT (sbalign), &ptop, &pbot, &pleft, &pright);
    gtk_alignment_set_padding (GTK_ALIGNMENT (sbalign), ptop, pbot + 2, pleft + 5, pright);

    statusbar = gtk_statusbar_new ();
    // status_set_default (app);

    gtk_container_add (GTK_CONTAINER (sbalign), statusbar);
    gtk_box_pack_end (GTK_BOX (vbox), sbalign, FALSE, FALSE, 0);

    gtk_widget_show (statusbar);
    gtk_widget_show (vbox);

    /* connect all signals */
    g_signal_connect (G_OBJECT (mwin->window), "delete-event", /* window del */
                      G_CALLBACK (on_window_delete_event), NULL);

    g_signal_connect (G_OBJECT (mwin->window), "destroy",    /* window dest  */
                      G_CALLBACK (on_window_destroy), NULL);

    /* general */
    /*
    g_signal_connect (G_OBJECT (app->view), "key_press_event",
                      G_CALLBACK (on_keypress), app);

    g_signal_connect (app->buffer, "changed",
                      G_CALLBACK (on_buffer_changed), app);

    g_signal_connect (app->buffer, "mark_set",
                      G_CALLBACK (on_mark_set), app);

    g_signal_connect (G_OBJECT (app->view), "toggle-overwrite",
                      G_CALLBACK (on_insmode), app);
    */

    /* File Menu */
    g_signal_connect (G_OBJECT (quitMi), "activate",        /* file Quit    */
                      G_CALLBACK (menu_file_quit_activate), NULL);

    /* Toolbar uses same menu callback */
    g_signal_connect (G_OBJECT (exit), "clicked",           /* file Quit    */
                      G_CALLBACK (menu_file_quit_activate), NULL);

    g_signal_connect (G_OBJECT (showtbMi), "activate",      /* show toolbar */
                      G_CALLBACK (menu_showtb_activate), mwin);

    g_signal_connect (G_OBJECT (showdocMi), "activate",      /* show toolbar */
                      G_CALLBACK (menu_showdoc_activate), mwin);

    g_signal_connect (selection, "changed",
                        G_CALLBACK (doctree_activate), mwin);

    /* set window title */
    gtk_window_set_title (GTK_WINDOW (mwin->window), "New GtkWrite Layout");
    // gtkwrite_window_set_title (NULL, app);
    gtk_widget_show (mwin->window);

    /* showall widgets */
    gtk_widget_show_all (mwin->window);

    /* TODO: load saved settings */
    // gtk_widget_set_visible (app->toolbar, app->showtoolbar);
    // gtk_widget_set_sensitive (app->tbappearMi, app->showtoolbar);


    return mwin->window;
}
