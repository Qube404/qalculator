fn main() {
    glib_build_tools::compile_resources(
        &["resources"],
        "resources/resources.gresource.xml",
        "templates_resources.gresource",
    );
    println!("cargo:rerun-if-changed=build.rs");
}
