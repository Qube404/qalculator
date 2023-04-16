mod qalculator_window;

use qalculator_window::QalculatorWindow;

use gtk::prelude::*;
use gtk::{gio, glib, Application};

const APP_ID: &str = "com.github.Qube404.Qalculator";

fn main() -> glib::ExitCode {
    gio::resources_register_include!("templates_resources.gresource")
        .expect("Failed to register resources");

    let app = Application::builder()
        .application_id(APP_ID)
        .build();

    app.connect_activate(activate);

    app.run()
}

fn activate(app: &Application) {
    let window = QalculatorWindow::new(app);
    window.present();
}
