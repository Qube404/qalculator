use std::cell::Cell;

use calc_lib::Equation;

use gtk::prelude::ObjectExt;
use gtk::subclass::prelude::*;
use gtk::{glib, CompositeTemplate, Button, Label};
use glib::subclass::InitializingObject;

#[derive(CompositeTemplate, Default)]
#[template(resource = "/com/github/Qube404/qalculator_window.ui")]
pub struct QalculatorWindow {
    #[template_child]
    pub label: TemplateChild<Label>,

    #[template_child]
    pub button1: TemplateChild<Button>,

    #[template_child]
    pub button2: TemplateChild<Button>,

    #[template_child]
    pub button3: TemplateChild<Button>,

    #[template_child]
    pub button4: TemplateChild<Button>,

    #[template_child]
    pub button5: TemplateChild<Button>,

    #[template_child]
    pub button6: TemplateChild<Button>,

    #[template_child]
    pub button7: TemplateChild<Button>,

    #[template_child]
    pub button8: TemplateChild<Button>,

    #[template_child]
    pub button9: TemplateChild<Button>,

    #[template_child]
    pub button0: TemplateChild<Button>,

    #[template_child]
    pub button_clear: TemplateChild<Button>,

    #[template_child]
    pub button_evaluate: TemplateChild<Button>,

    pub expression: Cell<String>,

    pub current_num: Cell<String>,
    pub current_op: Cell<String>
}

#[glib::object_subclass]
impl ObjectSubclass for QalculatorWindow {
    const NAME: &'static str = "QalculatorWindow";
    type Type = super::QalculatorWindow;
    type ParentType = gtk::ApplicationWindow;

    fn class_init(klass: &mut Self::Class) {
        klass.bind_template();
        klass.bind_template_callbacks();
    }

    fn instance_init(obj: &InitializingObject<Self>) {
        obj.init_template();
    }
}

#[gtk::template_callbacks]
impl QalculatorWindow {
    #[template_callback]
    fn append_num_to_expression(&self, button: &Button) {
        // Take value of the current expression.
        let mut current_string = self
            .expression
            .take();
        let mut current_num = self
            .current_num
            .take();

        // Get button value.
        let button_value = button
            .property_value("label");
        let button_value = button_value
            .get::<&str>()
            .unwrap();

        // Push button value.
        current_string.push_str(button_value);
        current_num.push_str(button_value);

        // Set value back to the taken expression which char pushed on.
        self
            .expression
            .set(current_string);
        self
            .current_num
            .set(current_num.clone());


        // Set button label to character.
        self
            .label
            .set_property("label", current_num);

        self.print_values();
    }

    #[template_callback]
    fn append_op_to_expression(&self, button: &Button) {
        // Clear current num
        self
            .current_num
            .set("".to_string());

        // Take value of the current expression.
        let mut current_string = self
            .expression
            .take();

        // Get button value.
        let button_value = button
            .property_value("label");
        let button_value = button_value
            .get::<&str>()
            .unwrap();

        // Push button value.
        current_string.push_str(button_value);

        // Set value back to the taken expression which char pushed on.
        self
            .expression
            .set(current_string.clone());
        self
            .current_op
            .set(button_value.to_string());

        // Set button label to character.
        self
            .label
            .set_property("label", button_value);

        self.print_values();
    }

    #[template_callback]
    fn clear_expression(&self, _button: &Button) {
        let cleared_value = String::from("0");

        self
            .expression
            .set(cleared_value.clone());

        self
            .label
            .set_property("label", cleared_value);
        self
            .current_num
            .set("".to_string());
        self
            .current_op
            .set("".to_string());

        self.print_values();
    }

    #[template_callback]
    fn evaluate_expression(&self, _button: &Button) {
        // Take value of the current expression.
        let current_string = self
            .expression
            .take();

        // Create new equation struct from current expression.
        let equ = match Equation::from(current_string.clone()) {
            Ok(val) => val,
            _ => {
                self
                    .expression
                    .set(current_string);
                return
            }
        };

        // Obtain equation result in type String.
        let equ_result = equ
            .get_result()
            .to_string();
        
        // Set expression to result.
        self
            .expression
            .set(equ_result.clone());
        self
            .current_num
            .set(equ_result.clone());

        self
            .label
            .set_property("label", equ_result);

        self.print_values();
    }

    fn print_values(&self) {
        let expression = self
            .expression
            .take();
        let num = self
            .current_num
            .take();
        let op = self
            .current_op
            .take();

        println!("Expression: {}\nNumber: {}\nOperator: {}", expression, num, op);

        self
            .expression
            .set(expression);
        self
            .current_num
            .set(num);
        self
            .current_op
            .set(op);
    }
}

impl ObjectImpl for QalculatorWindow {
    fn constructed(&self) {
        self.parent_constructed();
    }
}

impl WidgetImpl for QalculatorWindow {}

impl WindowImpl for QalculatorWindow {}

impl ApplicationWindowImpl for QalculatorWindow {}
