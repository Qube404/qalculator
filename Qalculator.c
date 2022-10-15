#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h> 
#include <stdio.h>
#include <string.h> 
#include <math.h> 
#define MAXNUM 10
#define MAXCHAR 20

typedef struct CalculatorState {
    char working_number1[MAXNUM];
    char working_number2[MAXNUM];
    char answer_number[MAXNUM];
    char working_text[MAXCHAR];
    char operator;
    int result_number;
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *grid;
    GtkWidget *label;
    GtkCssProvider *provider;
} CalculatorState;

CalculatorState global_state;

static void initialize_values(GtkApplication *app) {
    strcpy(global_state.answer_number, "0");
    strcpy(global_state.working_text, global_state.answer_number);
    global_state.window = gtk_application_window_new(app);
    global_state.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    global_state.grid = gtk_grid_new();
    global_state.label = gtk_label_new("0");
}

static void string_to_int(char *string) { 
    if(strlen(global_state.working_number1) >= MAXCHAR - 1) return;
    int number = 0;
    int i = strlen(string) - 1;
    g_print("string:%s\n", global_state.working_number1);
    int j = 0;
    while(i >= 0) {
        int thing = (string[i] - '0') * powl(10, j);
        number += (string[i] - '0') * powl(10, j);
        i--;
        j++;
    }
    g_print("%d\n", (int) strlen(global_state.working_number1));
}

static void append_to_working_number1(GtkWidget *window, int num_to_append) {
    char char_to_append = num_to_append + '0';

    strncat(global_state.working_number1, &char_to_append, 1);
    string_to_int(global_state.working_number1);
}

static void append_to_working_number2(GtkWidget *window, int num_to_append) {
    char char_to_append = num_to_append + '0';

    strncat(global_state.working_number2, &char_to_append, 1);
    string_to_int(global_state.working_number2);
}

static void create_calculator_grid(GtkWidget *window) {
    int number_of_buttons = 9;
    GtkWidget *number_buttons[number_of_buttons];

    gtk_widget_set_halign(global_state.grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(global_state.grid, GTK_ALIGN_CENTER);

    int i = 0;
    int j = 0;
    while(i <= number_of_buttons) {
        int calculator_number = i == number_of_buttons ? 0 : i + 1;

        char string_calculator_number[2] = {(char) calculator_number + '0', '\0'};
        number_buttons[i] = gtk_button_new_with_label(string_calculator_number);

        //g_signal_connect(buttons[i], "clicked", G_CALLBACK(append_to_working_number1), GINT_TO_POINTER(calculator_number));

        int row = i / 3;
        if(i == (row * 3)) j = 0;
        int column = j;

        gtk_grid_attach(GTK_GRID(global_state.grid), number_buttons[i], column, row, 1, 1);
        i++, j++;
    }
} 

static void create_operator_grid(GtkWidget *window) {
    int number_of_operators = 6;
    char string_of_operators[number_of_operators];
    strcpy(string_of_operators, "+-*/=.");

    GtkWidget *operator_buttons[number_of_operators];

    for(int i = 0; i <= strlen(string_of_operators) - 3; i++) {
        char current_operator[2] = {string_of_operators[i]};
        operator_buttons[i] = gtk_button_new_with_label(current_operator);
        char *thing = &string_of_operators[i];
        gtk_grid_attach(GTK_GRID(global_state.grid), operator_buttons[i], 3, i, 1, 1);

        if(i == strlen(string_of_operators) - 3) {
            for(int j = 1; j <= 2; j++) {
                char current_operator[2] = {string_of_operators[i + j]};
                operator_buttons[i + j] = gtk_button_new_with_label(current_operator);
                gtk_grid_attach(GTK_GRID(global_state.grid), operator_buttons[i + j], i - j, i, 1, 1);
            }
        }
    }
}

static void css_set(GtkCssProvider *provider, GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void activate(GtkApplication *app, gpointer user_data) {
    initialize_values(app);

    global_state.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(global_state.window), "Qalculator");
    gtk_window_set_default_size(GTK_WINDOW(global_state.window), 500, 500);

    gtk_window_set_child(GTK_WINDOW(global_state.window), global_state.vbox);
    gtk_widget_set_halign(global_state.vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(global_state.vbox, GTK_ALIGN_CENTER);

    create_calculator_grid(global_state.vbox);
    create_operator_grid(global_state.vbox);

    gtk_box_append(GTK_BOX(global_state.vbox), global_state.label);
    gtk_box_append(GTK_BOX(global_state.vbox), global_state.grid);

    // CSS Linking
    //global_state.provider = gtk_css_provider_new();
    //gtk_css_provider_load_from_path(global_state.provider, "style.css");
    //css_set(global_state.provider, global_state.label);

    gtk_widget_show(global_state.window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.github.Qube404.Qalculator", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
