#!/bin/bash
set -e

DOWNLOAD_DIR="./downloads"
TIVAWARE_INSTALLER="${DOWNLOAD_DIR}/SW-TM4C-2.2.0.295.exe"
EXTRACTED_DIR="${DOWNLOAD_DIR}/tivaware_extracted"
TIVAWARE_DIR="./libraries/TivaWare"

echo "Creating directories..."
mkdir -p "${DOWNLOAD_DIR}"
mkdir -p "${EXTRACTED_DIR}"
mkdir -p "${TIVAWARE_DIR}"

# Check if tivaware_extracted already has the files
if [ -d "${EXTRACTED_DIR}" ] && [ "$(ls -A ${EXTRACTED_DIR})" ]; then
    echo "TivaWare files already extracted in ${EXTRACTED_DIR}"
else
    echo "Downloading TivaWare SDK..."
    echo "Note: You'll need to manually download the TivaWare SDK from the TI website."
    echo "Visit: https://www.ti.com/tool/SW-TM4C"
    echo "Register/Login and download SW-TM4C-2.2.0.295.exe"
    echo "Save it to: ${DOWNLOAD_DIR}/"
    read -p "Press Enter once you've downloaded the SDK file..." 

    if [ ! -f "${TIVAWARE_INSTALLER}" ]; then
        echo "Error: TivaWare SDK installer not found at ${TIVAWARE_INSTALLER}"
        echo "Please download it manually and place it in the ${DOWNLOAD_DIR} directory."
        exit 1
    fi

    echo "Extracting TivaWare SDK..."
    # The TivaWare SDK is a self-extracting Windows executable
    # We'll use 7z or wine to extract it
    if command -v 7z &> /dev/null; then
        7z x -o"${EXTRACTED_DIR}" "${TIVAWARE_INSTALLER}"
    elif command -v wine &> /dev/null; then
        cd "${DOWNLOAD_DIR}" && wine "${TIVAWARE_INSTALLER}" /auto
    else
        echo "Error: Neither 7z nor wine is installed. Please install one of them to extract the TivaWare SDK."
        echo "For example: sudo apt-get install p7zip-full"
        exit 1
    fi
fi

echo "Copying TivaWare SDK to libraries directory..."

# Check for required directories and copy them
REQUIRED_DIRS=("driverlib" "inc" "utils")
for dir in "${REQUIRED_DIRS[@]}"; do
    if [ -d "${EXTRACTED_DIR}/${dir}" ]; then
        echo "Copying ${dir} directory..."
        cp -r "${EXTRACTED_DIR}/${dir}" "${TIVAWARE_DIR}/"
    else
        echo "Warning: Required ${dir} directory not found in extracted files."
    fi
done

# Copy additional useful directories if they exist
OPTIONAL_DIRS=("boot_loader" "grlib" "usblib" "sensorlib" "third_party")
for dir in "${OPTIONAL_DIRS[@]}"; do
    if [ -d "${EXTRACTED_DIR}/${dir}" ]; then
        echo "Copying ${dir} directory..."
        cp -r "${EXTRACTED_DIR}/${dir}" "${TIVAWARE_DIR}/"
    fi
done

# Copy makefiles and other important files
if [ -f "${EXTRACTED_DIR}/makedefs" ]; then
    cp "${EXTRACTED_DIR}/makedefs" "${TIVAWARE_DIR}/"
fi
if [ -f "${EXTRACTED_DIR}/Makefile" ]; then
    cp "${EXTRACTED_DIR}/Makefile" "${TIVAWARE_DIR}/"
fi

echo "TivaWare SDK setup complete!"
echo "The SDK is now in ${TIVAWARE_DIR}"