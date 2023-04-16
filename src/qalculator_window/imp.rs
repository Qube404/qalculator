use glib::subclass::InitializingObject;
use gtk::subclass::prelude::*;
use gtk::{glib, CompositeTemplate};

#[derive(CompositeTemplate, Default)]
#[template(resource = "/com/github/Qube404/qalculator_window.ui")]
pub struct QalculatorWindow {
}

#[glib::object_subclass]
impl ObjectSubclass for QalculatorWindow {
    const NAME: &'static str = "QalculatorWindow";
    type Type = super::QalculatorWindow;
    type ParentType = gtk::ApplicationWindow;

    fn class_init(klass: &mut Self::Class) {
        klass.bind_template();
    }

    fn instance_init(obj: &InitializingObject<Self>) {
        obj.init_template();
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
