#include <gtk/gtk.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <math.h> 
#define MAXNUMLEN 5 
#define MAXNUMSTR "32767"
#define MAXCHAR 5 

typedef struct CalculatorState {
    char *working_number;
    char number1[MAXNUMLEN];
    char number2[MAXNUMLEN];
    char answer_number[MAXNUMLEN];
    char working_text[MAXCHAR];
    char operator;
    int flags;
    int result_number;
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *grid;
    GtkWidget *label;
    GtkCssProvider *provider;
} CalculatorState;

CalculatorState global_state;

static void initialize_values(GtkApplication *app) {
    global_state.working_number = global_state.number1;
    strcpy(global_state.answer_number, "0");
    strcpy(global_state.working_text, global_state.answer_number);
    strcpy(&global_state.operator, "\0");
    global_state.window = gtk_application_window_new(app);
    global_state.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    global_state.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    global_state.grid = gtk_grid_new();
    global_state.label = gtk_label_new("0");
}

static int string_to_int(char *string) { 
    char *copy_of_string = string;

    if(strlen(global_state.number1) >= MAXCHAR - 1) return -1;
    int number = 0;
    int i = strlen(copy_of_string) - 1;
    int j = 0;
    int thing;
    while(i >= 0) {
        thing = (copy_of_string[i] - '0') * powl(10, j);
        number += (copy_of_string[i] - '0') * powl(10, j);
        i--;
        j++;
    }

    return number;
}

// Behaviour for clicking button of type number.
static void append_to_working_number(GtkWidget *window, int num_to_append) {
    char char_to_append = num_to_append + '0';
    char *temp_string = global_state.working_number;

    strncat(temp_string, &char_to_append, 1);
    if(strlen(temp_string) >= strlen(MAXNUMSTR)) {
        for(int i = 0; i <= strlen(temp_string); i++) {
            if(string_to_int(&temp_string[i]) >= string_to_int(&MAXNUMSTR[i])) {
                return;
            }
        }
    } 

    global_state.working_number = temp_string;
    gtk_label_set_label(GTK_LABEL(global_state.label), global_state.working_number);

    g_print("number 1 & 2: %s %s\nresult_number: %d\n\n", global_state.number1, global_state.number2, global_state.result_number);
}

// Handler for clicking a special button that is not a normal number button.
static void operator_button_behaviour(GtkWidget *window, gpointer *pointer_operator) {
    char operators[6] = "+-*/=.";
    char operator = GPOINTER_TO_INT(pointer_operator);
    char string_ver_operator[] = {operator, '\0'};

    gtk_label_set_label(GTK_LABEL(global_state.label), string_ver_operator);
    global_state.working_number = global_state.number2;

    if(operator != '=') {
        global_state.operator = operator;
    } else {
        switch(global_state.operator) {
            case '+':
                global_state.result_number = string_to_int(global_state.number1) + string_to_int(global_state.number2);
                break;
            case '-':
                global_state.result_number = string_to_int(global_state.number1) - string_to_int(global_state.number2);
                break;
            case '*':
                global_state.result_number = string_to_int(global_state.number1) * string_to_int(global_state.number2);
                break;
            case '/':
                global_state.result_number = string_to_int(global_state.number1) / string_to_int(global_state.number2);
                break;
            default:
                return;
            }

            global_state.working_number = global_state.number1;
            sprintf(global_state.working_number, "%d", global_state.result_number);
            strcpy(global_state.number2, "\0"); 
            gtk_label_set_label(GTK_LABEL(global_state.label), global_state.working_number);
    }
}

static void special_button_behaviour(GtkWidget *window, gpointer *pointer_operator) {
    char operator = GPOINTER_TO_INT(pointer_operator);
    char string_ver_operator[] = {operator, '\0'};

    switch(operator) {
        case 'C':
                gtk_label_set_label(GTK_LABEL(global_state.label), "0");
                strcpy(global_state.number1, "\0");
                strcpy(global_state.number2, "\0");
                strcpy(global_state.working_number, "\0");
                strcpy(global_state.working_text, "0");
                strcpy(global_state.answer_number, "0");
                global_state.result_number = 0;
                break;
        case 'X':
                int last_char = strlen(global_state.working_number); 
                global_state.working_number[last_char - 1] = '\0';

                if(last_char <= 1) {
                    gtk_label_set_label(GTK_LABEL(global_state.label), "0");
                } else {
                    gtk_label_set_label(GTK_LABEL(global_state.label), global_state.working_number);
                }
                break;
        default:
            return;
    }
}

static void create_special_buttons() {
    int number_of_buttons = 2;
    GtkWidget *number_buttons[number_of_buttons];
    char *labels[] = {"C", "X"};

    number_buttons[0] = gtk_button_new_with_label(labels[0]);
    number_buttons[1] = gtk_button_new_with_label(labels[1]);

    gtk_widget_set_halign(global_state.hbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(global_state.hbox, GTK_ALIGN_CENTER);

    g_signal_connect(number_buttons[0], "clicked", G_CALLBACK(special_button_behaviour), GINT_TO_POINTER(labels[0][0]));
    g_signal_connect(number_buttons[1], "clicked", G_CALLBACK(special_button_behaviour), GINT_TO_POINTER(labels[1][0]));

    gtk_box_append(GTK_BOX(global_state.hbox), number_buttons[0]);
    gtk_box_append(GTK_BOX(global_state.hbox), number_buttons[1]);
}

static void create_calculator_grid() {
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

        g_signal_connect(number_buttons[i], "clicked", G_CALLBACK(append_to_working_number), GINT_TO_POINTER(calculator_number));

        int row = i / 3;
        if(i == (row * 3)) j = 0;
        int column = j;

        gtk_grid_attach(GTK_GRID(global_state.grid), number_buttons[i], column, row, 1, 1);
        i++, j++;
    }
} 

static void create_operator_grid() {
    int number_of_operators = 6;
    char string_of_operators[number_of_operators];
    strcpy(string_of_operators, "+-*/=.");

    GtkWidget *operator_buttons[number_of_operators];

    for(int i = 0; i <= strlen(string_of_operators) - 3; i++) {
        char current_operator[1] = {string_of_operators[i]};
        operator_buttons[i] = gtk_button_new_with_label(current_operator);
        g_signal_connect(operator_buttons[i], "clicked", G_CALLBACK(operator_button_behaviour), GINT_TO_POINTER(current_operator[0]));
        char *thing = &string_of_operators[i];
        gtk_grid_attach(GTK_GRID(global_state.grid), operator_buttons[i], 3, i, 1, 1);

        if(i == strlen(string_of_operators) - 3) {
            for(int j = 1; j <= 2; j++) {
                char current_operator[2] = {string_of_operators[i + j]};
                operator_buttons[i + j] = gtk_button_new_with_label(current_operator);
                g_signal_connect(operator_buttons[i + j], "clicked", G_CALLBACK(operator_button_behaviour), GINT_TO_POINTER(current_operator[0]));
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

    gtk_window_set_title(GTK_WINDOW(global_state.window), "Qalculator");
    gtk_window_set_default_size(GTK_WINDOW(global_state.window), 210, 210);

    gtk_window_set_child(GTK_WINDOW(global_state.window), global_state.vbox);
    gtk_widget_set_halign(global_state.vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(global_state.vbox, GTK_ALIGN_CENTER);

    create_special_buttons();
    create_calculator_grid();
    create_operator_grid();

    gtk_box_append(GTK_BOX(global_state.vbox), global_state.hbox);
    gtk_box_append(GTK_BOX(global_state.vbox), global_state.label);
    gtk_box_append(GTK_BOX(global_state.vbox), global_state.grid);

    // CSS Linking
    global_state.provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(global_state.provider, "style.css");
    css_set(global_state.provider, global_state.label);

    gtk_window_present(GTK_WINDOW(global_state.window));
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
