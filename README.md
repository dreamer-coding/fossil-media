# **Fossil Media by Fossil Logic**

**Fossil Media** is a lightweight, portable data parsing and processing library written in pure C with zero external dependencies. Designed for maximum portability and flexibility, Fossil Media provides robust parsers and utilities to handle a variety of common and complex data formats including JSON, CSV, XML, and more. Its minimal footprint and clean, audit-friendly codebase make it ideal for embedded, cross-platform, and security-sensitive applications that require reliable data ingestion and transformation.

### Key Features

- **Wide Format Support**  
  Parse and process popular data formats such as JSON, CSV, XML, and extendable to custom formats.

- **Cross-Platform Compatibility**  
  Runs effortlessly on Windows, macOS, Linux, and embedded systems without modification.

- **Zero External Dependencies**  
  Written entirely in clean, portable C to simplify integration and enable thorough code audits.

- **Efficient and Lightweight**  
  Optimized for minimal memory and CPU usage, making it perfect for resource-constrained environments.

- **Modular and Extensible**  
  Easily customizable to support new data formats or tailor parsing behavior to your project’s needs.

- **Robust Data Handling**  
  Supports streaming parsing, validation, and conversion utilities for flexible data workflows.

## Getting Started

### Prerequisites

- **Meson Build System**  
  Fossil Media uses Meson for build configuration. If you don’t have Meson installed, please follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Fossil Media as a Dependency

#### Using Meson

### **Install or Upgrade Meson** (version 1.3 or newer recommended):

```sh
   python -m pip install meson           # Install Meson
   python -m pip install --upgrade meson # Upgrade Meson
```
###	Add the .wrap File
Place a file named fossil-media.wrap in your subprojects directory with the following content:

```ini
# ======================
# Git Wrap package definition
# ======================
[wrap-git]
url = https://github.com/fossillogic/fossil-media.git
revision = v0.1.0

[provide]
fossil-media = fossil_media_dep
```

###	Integrate in Your meson.build
Add the dependency by including this line:

```meson
media_dep = dependency('fossil-media')
```


## Build Configuration Options

Customize your build with the following Meson options:
	•	Enable Tests
To run the built-in test suite, configure Meson with:

```sh
meson setup builddir -Dwith_test=enabled
```

## Contributing and Support

For those interested in contributing, reporting issues, or seeking support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are always welcome.