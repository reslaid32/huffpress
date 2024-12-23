#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

BIN_DIR="/usr/local/bin"
# Target directory for the libraries
LIB_DIR="/usr/local/lib"

# Directory containing the .so files and binaries
SOURCE_DIR="./build/bin"

# Define the list of files to copy
LIBS=("libhuffchecksum.so" "libhuffman.so" "libhuffpress.so" "libhuffpresscli.so")
BINS=("hpfcli")

# Function to check for root privileges
check_root() {
  if [ "$EUID" -ne 0 ]; then
    echo "Please run this script with sudo or as root."
    exit 1
  fi
}

# Function to install libraries
install_libraries() {
  echo "The following library files will be installed to $LIB_DIR:"
  for FILE in "${LIBS[@]}"; do
    echo "$SOURCE_DIR/$FILE"
  done
  echo

  # Confirm installation
  read -p "Do you want to proceed with installing libraries? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Copy the libraries
  echo "Copying library files to $LIB_DIR..."
  for FILE in "${LIBS[@]}"; do
    if [ -f "$SOURCE_DIR/$FILE" ]; then
      cp "$SOURCE_DIR/$FILE" "$LIB_DIR"
    else
      echo "Warning: $SOURCE_DIR/$FILE not found. Skipping."
    fi
  done

  # Update the linker cache
  echo "Updating the dynamic linker cache..."
  ldconfig

  echo "Library installation complete!"
}

# Function to install binaries
install_binaries() {
  echo "The following binary files will be installed to $BIN_DIR:"
  for FILE in "${BINS[@]}"; do
    echo "$SOURCE_DIR/$FILE"
  done
  echo

  # Confirm installation
  read -p "Do you want to proceed with installing binaries? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Copy the binaries
  echo "Copying binary files to $BIN_DIR..."
  for FILE in "${BINS[@]}"; do
    if [ -f "$SOURCE_DIR/$FILE" ]; then
      cp "$SOURCE_DIR/$FILE" "$BIN_DIR"
    else
      echo "Warning: $SOURCE_DIR/$FILE not found. Skipping."
    fi
  done

  echo "Binary installation complete!"
}

# Function to uninstall libraries
uninstall_libraries() {
  echo "The following library files will be removed from $LIB_DIR:"
  for FILE in "${LIBS[@]}"; do
    echo "$LIB_DIR/$FILE"
  done
  echo

  # Confirm uninstallation
  read -p "Do you want to proceed with uninstalling libraries? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Remove the libraries
  echo "Removing library files from $LIB_DIR..."
  for FILE in "${LIBS[@]}"; do
    if [ -f "$LIB_DIR/$FILE" ]; then
      rm "$LIB_DIR/$FILE"
      echo "Removed $LIB_DIR/$FILE"
    else
      echo "Warning: $LIB_DIR/$FILE not found. Skipping."
    fi
  done

  # Update the linker cache
  echo "Updating the dynamic linker cache..."
  ldconfig

  echo "Library uninstallation complete!"
}

# Function to uninstall binaries
uninstall_binaries() {
  echo "The following binary files will be removed from $BIN_DIR:"
  for FILE in "${BINS[@]}"; do
    echo "$BIN_DIR/$FILE"
  done
  echo

  # Confirm uninstallation
  read -p "Do you want to proceed with uninstalling binaries? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Remove the binaries
  echo "Removing binary files from $BIN_DIR..."
  for FILE in "${BINS[@]}"; do
    if [ -f "$BIN_DIR/$FILE" ]; then
      rm "$BIN_DIR/$FILE"
      echo "Removed $BIN_DIR/$FILE"
    else
      echo "Warning: $BIN_DIR/$FILE not found. Skipping."
    fi
  done

  echo "Binary uninstallation complete!"
}

# Function to reinstall libraries and binaries
reinstall_libraries_and_binaries() {
  uninstall_libraries
  uninstall_binaries
  install_libraries
  install_binaries
}

# Main script logic
case "$1" in
  install)
    check_root
    install_libraries
    install_binaries
    ;;
  uninstall)
    check_root
    uninstall_libraries
    uninstall_binaries
    ;;
  reinstall)
    check_root
    reinstall_libraries_and_binaries
    ;;
  *)
    echo "Usage: $0 {install|uninstall|reinstall}"
    exit 1
    ;;
esac
