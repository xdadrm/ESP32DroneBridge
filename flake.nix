{
  description = "DroneBridge ESP32 Development Environment with PPM Integration";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    nixpkgs-esp-dev.url = "github:mirrexagon/nixpkgs-esp-dev";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, nixpkgs-esp-dev, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ nixpkgs-esp-dev.overlays.default ];
          config.allowUnfree = true;
        };

        esp32c3Shell = pkgs.mkShell {
          name = "dronebridge-esp32c3-dev";

          buildInputs = with pkgs; [
            # ESP32-C3 toolchain
            esp-idf-esp32c3
            platformio-core
            python311
            nodejs_20
            
            # Build tools
            cmake ninja gnumake gcc gdb
            
            # Serial tools
            esptool minicom picocom
            
            # Development utilities
            git curl wget pkg-config
          ];

          shellHook = ''
            export IDF_PATH="${pkgs.esp-idf-esp32c3}"
            export PLATFORMIO_CORE_DIR="$PWD/.platformio"
            export CONFIG_PPM_ENABLED="1"
            export CONFIG_PPM_GPIO_PIN="4"
            
            echo "🚁 DroneBridge ESP32-C3 Development Environment"
            echo "Target Platform: esp32c3"
            echo "ESP-IDF Path: $IDF_PATH"
            
            # Source ESP-IDF environment
            if [ -f "$IDF_PATH/export.sh" ]; then
              source "$IDF_PATH/export.sh" >/dev/null 2>&1
              echo "ESP-IDF environment loaded successfully!"
            fi
            
            echo "Environment ready! 🎯"
          '';
        };
      in
      {
        devShells = {
          default = esp32c3Shell;
          esp32c3 = esp32c3Shell;
        };
      }
    );
}

