#include <gtk/gtk.h>

/*gcc -Wall -L /usr/lib -I /usr/include/webkit-1.0/ -g main.c -o main `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0 webkit-1.0`*/

static GtkWidget* main_window;
static GtkWidget* uri_entry;
static GtkStatusbar* main_statusbar;
static guint status_context_id;
static GtkTextBuffer *buffer;
static PangoFontDescription *font;


static void
destroy_cb (GtkWidget* widget, gpointer data)
{
    gtk_main_quit ();
}

static GtkWidget*
create_browser ()
{
    GtkWidget* textview = gtk_text_view_new();
	font = pango_font_description_from_string ("Monospace Bold 10");
	
	gtk_widget_modify_font (textview, font);
	
	/* setting how text should be arranged in a browser */
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
	gtk_text_view_set_justification (GTK_TEXT_VIEW (textview), GTK_JUSTIFY_LEFT);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview), TRUE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview), TRUE);
	gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_pixels_inside_wrap (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (textview), 10);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (textview), 10);
	
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
	gtk_text_buffer_set_text (buffer, "This is a test browser", -1);
	GtkWidget* scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
	GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	gtk_container_add (GTK_CONTAINER(scrolled_win), textview);
    return scrolled_win;
}

static GtkWidget*
create_statusbar ()
{
    main_statusbar = GTK_STATUSBAR (gtk_statusbar_new ());
    status_context_id = gtk_statusbar_get_context_id (main_statusbar, "Link Hover");

    return (GtkWidget*)main_statusbar;
}

static GtkWidget*
create_toolbar ()
{
    GtkWidget* toolbar = gtk_toolbar_new ();

    gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH_HORIZ);

    GtkToolItem* item;

    /* the back button */
    item = gtk_tool_button_new_from_stock (GTK_STOCK_GO_BACK);
   // g_signal_connect (G_OBJECT (item), "clicked", G_CALLBACK (), NULL);
    gtk_toolbar_insert (GTK_TOOLBAR (toolbar), item, -1);

    /* The forward button */
    item = gtk_tool_button_new_from_stock (GTK_STOCK_GO_FORWARD);
    g_signal_connect (G_OBJECT (item), "clicked", G_CALLBACK (), NULL);
    gtk_toolbar_insert (GTK_TOOLBAR (toolbar), item, -1);

    /* The URL entry */
    item = gtk_tool_item_new ();
    gtk_tool_item_set_expand (item, TRUE);
    uri_entry = gtk_entry_new ();
    gtk_container_add (GTK_CONTAINER (item), uri_entry);
   // g_signal_connect (G_OBJECT (uri_entry), "activate", G_CALLBACK (), NULL);
    gtk_toolbar_insert (GTK_TOOLBAR (toolbar), item, -1);

    /* The go button */
    item = gtk_tool_button_new_from_stock (GTK_STOCK_OK);
   // g_signal_connect_swapped (G_OBJECT (item), "clicked", G_CALLBACK (), (gpointer)uri_entry);
    gtk_toolbar_insert (GTK_TOOLBAR (toolbar), item, -1);

    return toolbar;
}

static GtkWidget*
create_window ()
{
	static GdkColor color;
	
    gdk_color_parse("black", &color);
    GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_fg(window, GTK_STATE_NORMAL, &color);
    gtk_widget_set_name (window, "GtkLauncher");
    gtk_window_set_title(GTK_WINDOW(window), "Mortiff");
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy_cb), NULL);

    return window;
}

int
main (int argc, char* argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget* vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), create_statusbar (), FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), create_toolbar (), FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), create_browser (), TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), create_statusbar (), FALSE, FALSE, 0);

    main_window = create_window ();
    gtk_container_add (GTK_CONTAINER (main_window), vbox);
    
    gtk_widget_show_all (main_window);
    gtk_main ();

    return 0;
}
