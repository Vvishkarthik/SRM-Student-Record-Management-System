# 🚀 SRM Student Record Management System  
### A C Programming Project with Role-Based Access Control

<div align="center">

![C Language](https://img.shields.io/badge/Language-C-blue.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)
![Database](https://img.shields.io/badge/Storage-File%20Handling-orange?style=for-the-badge)

</div>

---

## 📖 Introduction
A secure & efficient **Student Record Management System (SRMS)** created using C programming.  
It uses **file handling** to store student details permanently and supports role-based access system  
(Admin, Staff & Guest) similar to real academic software. 🎓

---

## 🎯 Features

| Feature | Admin | Staff | Guest |
|--------|:----:|:----:|:----:|
| Add Student | ✔ | ✖ | ✖ |
| View Students | ✔ | ✔ | ✔ |
| Search Student | ✔ | ✔ | ✔ |
| Edit Student | ✔ | ✔ | ✖ |
| Delete Student | ✔ | ✖ | ✖ |

✔ CGPA based grade calculation  
✔ Duplicate roll number validation  
✔ Text file storage for persistence  
✔ Clean tabular display  

---

## 🗂️ Student Data Format Stored

Each student record contains:

- Roll Number (unique)
- Name
- Department (CSE, ECE, EEE, MECH, CIVIL)
- Year (1–4)
- CGPA (0.0 – 10.0)
- Grade (auto computed)

Example in `students.txt`:
