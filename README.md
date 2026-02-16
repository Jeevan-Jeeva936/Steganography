# 🖼️ Image Steganography in C

This project implements **Image Steganography** using the C programming language.  
It hides secret data inside a BMP image file and can also extract the hidden data back from the image.

This project was developed as part of a systems programming / file handling practice to understand low-level binary data manipulation in C.

---

## 📌 Features

- Hide secret text inside a BMP image
- Extract hidden text from the stego image
- Works using Least Significant Bit (LSB) technique
- Command-line based tool
- Efficient binary file handling using `fread`, `fwrite`, and file pointers

---

## 🧠 Concept Used

The project uses **LSB (Least Significant Bit) Steganography**:
- Each byte of secret data is stored in the least significant bits of image pixels
- Human eye cannot detect the visual difference
- Allows secure hidden communication

---

## 📂 Project Structure

Steganography/
│── encode.c # Encoding logic (hide data)
│── decode.c # Decoding logic (extract data)
│── types.h # Custom data types and structures
│── common.h # Common definitions
│── secret.txt # Input secret message
│── output.bmp # Encoded stego image
│── stego.bmp # Generated image after encoding

👨‍💻 Author

Jeevan N
ECE Student | C Developer | Embedded & Systems Programming Enthusiast
