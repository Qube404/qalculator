mod qalculator_window;

use gtk::gdk::Display;
use qalculator_window::QalculatorWindow;

use gtk::{prelude::*, CssProvider, style_context_add_provider_for_display};
use gtk::{gio, glib, Application};

const APP_ID: &str = "com.github.Qube404.Qalculator";

fn main() -> glib::ExitCode {
    gio::resources_register_include!("templates_resources.gresource")
        .expect("Failed to register resources");

    let app = Application::builder()
        .application_id(APP_ID)
        .build();

    app.connect_startup(|_| load_css());
    app.connect_activate(activate);

    app.run()
}

fn load_css() {
    let provider = CssProvider::new();
    provider.load_from_path("resources/styles.css");

    style_context_add_provider_for_display(
        &Display::default().expect("Could not connect to a display."), 
        &provider, 
        gtk::STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

fn activate(app: &Application) {
    let window = QalculatorWindow::new(app);
    window.present();
}
