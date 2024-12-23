#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Target directory for the libraries
LIB_DIR="/usr/local/lib"

# Directory containing the .so files
SOURCE_DIR="./build/bin"

# Define the list of files to copy
FILES=("libhuffchecksum.so" "libhuffman.so" "libhuffpress.so" "libhuffpresscli.so")

# Function to check for root privileges
check_root() {
  if [ "$EUID" -ne 0 ]; then
    echo "Please run this script with sudo or as root."
    exit 1
  fi
}

# Function to install libraries
install_libraries() {
  echo "The following files will be installed to $LIB_DIR:"
  for FILE in "${FILES[@]}"; do
    echo "$SOURCE_DIR/$FILE"
  done
  echo

  # Confirm installation
  read -p "Do you want to proceed? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Copy the files
  echo "Copying files to $LIB_DIR..."
  for FILE in "${FILES[@]}"; do
    if [ -f "$SOURCE_DIR/$FILE" ]; then
      cp "$SOURCE_DIR/$FILE" "$LIB_DIR"
    else
      echo "Warning: $SOURCE_DIR/$FILE not found. Skipping."
    fi
  done

  # Update the linker cache
  echo "Updating the dynamic linker cache..."
  ldconfig

  echo "Installation complete!"
}

# Function to uninstall libraries
uninstall_libraries() {
  echo "The following files will be removed from $LIB_DIR:"
  for FILE in "${FILES[@]}"; do
    echo "$LIB_DIR/$FILE"
  done
  echo

  # Confirm uninstallation
  read -p "Do you want to proceed? (y/n): " CONFIRM
  if [[ "$CONFIRM" != "y" ]]; then
    echo "Operation canceled."
    exit 0
  fi

  # Remove the files
  echo "Removing files from $LIB_DIR..."
  for FILE in "${FILES[@]}"; do
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

  echo "Uninstallation complete!"
}

# Function to reinstall libraries
reinstall_libraries() {
  uninstall_libraries
  install_libraries
}

# Main script logic
case "$1" in
  install)
    check_root
    install_libraries
    ;;
  uninstall)
    check_root
    uninstall_libraries
    ;;
  reinstall)
    check_root
    reinstall_libraries
    ;;
  *)
    echo "Usage: $0 {install|uninstall|reinstall}"
    exit 1
    ;;
esac
