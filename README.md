# 🔐 Image Steganography using C

## 🔹 Project Overview

The **Image Steganography** project is a command-line application developed in **C** that hides and retrieves secret text messages inside a BMP image using the **Least Significant Bit (LSB)** technique.
This project demonstrates how data can be securely embedded within images without visibly altering the image.

It focuses on core concepts such as **file handling, bit manipulation, pointers, and structures**, making it an excellent project for understanding low-level data encoding and decoding.

---

## 🚀 Features

* 🔒 Encode (hide) secret text inside a BMP image
* 🔍 Decode (extract) hidden message from stego image
* 📁 Supports 24-bit BMP image format
* 🧩 Uses LSB technique for data hiding
* 💾 Maintains image quality after encoding
* 🛠️ Command-line based interface

---

## 🛠️ Tech Stack

* **Language:** C
* **Concepts Used:**

  * File Handling (`fopen`, `fread`, `fwrite`)
  * Bitwise Operations
  * Structures
  * Pointers
  * Modular Programming

---

## 📂 Project Structure

```
Steganography/
│── encode.c
│── decode.c
│── common.h
│── encode.h
│── decode.h
│── main.c
│── beautiful.bmp
│── secret.txt
│── stego.bmp
│── a.exe
│── README.md
```

*(File names may vary slightly based on implementation)*

---

## ⚙️ How It Works

### 🔐 Encoding Process

1. Read the original BMP image
2. Convert secret message into binary
3. Embed message bits into LSB of image pixels
4. Generate a new stego image containing hidden data

### 🔍 Decoding Process

1. Read the stego image
2. Extract LSB bits from pixels
3. Reconstruct the hidden message
4. Display or save the decoded text

---

## 🧪 How to Compile & Run

### 1️⃣ Compile

```bash
gcc *.c -o stego
```

### 2️⃣ Encode Message

```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```

### 3️⃣ Decode Message

```bash
./stego -d stego.bmp output.txt
```

(On Windows)

```bash
stego.exe -e beautiful.bmp secret.txt stego.bmp
stego.exe -d stego.bmp output.txt
```

---

## 📌 Command-Line Arguments

| Option | Description                      |
| ------ | -------------------------------- |
| `-e`   | Encode secret message into image |
| `-d`   | Decode hidden message from image |

---

## 🧠 Learning Outcomes

This project demonstrates:

* Data hiding using LSB technique
* Bit-level manipulation in C
* Working with binary image files
* Real-world application of file I/O operations
* Modular and structured C programming

---

## 🔮 Future Enhancements

* Support for PNG/JPEG formats
* Password-protected steganography
* GUI-based application
* Support for hiding multiple files

---

## 👨‍💻 Author

**Jeevan N**
GitHub: https://github.com/Jeevan-Jeeva936

---

## 📜 License

This project is developed for educational purposes and is free to use and modify.
