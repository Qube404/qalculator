mod imp;

use gtk::{gio, glib, Application};
use glib::Object;

glib::wrapper! {
    pub struct QalculatorWindow(ObjectSubclass<imp::QalculatorWindow>)
        @extends gtk::ApplicationWindow, gtk::Window, gtk::Widget,
        @implements gio::ActionGroup, gio::ActionMap, gtk::Accessible, gtk::Buildable,
                    gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl QalculatorWindow {
    pub fn new(app: &Application) -> Self {
        Object::builder()
            .property("application", app)
            .build()
    }
}
