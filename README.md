# SegFaulted

> Reimplementations of **Rainfall** and **Override**, two security-focused projects from the 42 School curriculum. The original exercises are written in x86 Assembly; this repository contains reconstructions in **C** and **C++**.

---

## 📁 Repository Structure

```
segfaulted/
├── rainfall/
│   ├── level0/
│   │   ├── source.c
│   │   └── walkthrough.md
│   ├── level1/
│   │   ├── source.c
│   │   └── walkthrough.md
│   ├── ...
│   └── README.md
├── override/
│   ├── level0/
│   │   ├── source.c       (or source.cpp)
│   │   └── walkthrough.md
│   ├── level1/
│   │   ├── source.cpp
│   │   └── walkthrough.md
│   ├── ...
│   └── README.md
└── README.md  ← you are here
```

---

## 🌧️ Rainfall

Rainfall is a reverse engineering / exploitation project. Each level is a SUID binary; the goal is to find and exploit a vulnerability to retrieve the password for the next level.

**Key concepts covered:**
- Buffer overflows (stack-based)
- Format string vulnerabilities
- Return-oriented programming (ROP)
- GOT/PLT overwrites
- Shellcode injection
- `ret2libc` attacks

The source files in this repo are **reverse-engineered reconstructions** of the original binaries, written in C to match the disassembled logic as closely as possible.

---

## 🔓 Override

Override is the successor to Rainfall. It builds on the same reverse engineering and exploitation methodology with more complex binaries and additional vulnerability classes.

**Key concepts covered:**
- C++ object internals and vtable exploitation
- Heap-based overflows
- Use-after-free
- Off-by-one errors
- Advanced format string exploitation
- ASLR bypass techniques

Source files are reconstructed in both C and C++ depending on the original binary's language, as identified through decompilation.

---

## 🛠️ How to Use

Each level folder contains:

| File | Description |
|------|-------------|
| `source.c` / `source.cpp` | Reconstructed source matching the original binary |
| `walkthrough.md` | Explanation of the vulnerability and the exploit used |

To compile a level locally (for study purposes):

```bash
# C
gcc -m32 -fno-stack-protector -z execstack -o level0 source.c

# C++
g++ -m32 -fno-stack-protector -z execstack -o level0 source.cpp
```

> ⚠️ These flags intentionally disable security mitigations to reproduce the vulnerable environment used in the original project.

---

## 🧠 Notes

- The original binaries run on a **32-bit Linux** environment — compile with `-m32` accordingly.
- Reconstructed sources aim to produce the same assembly as the original when compiled with matching flags; minor differences may exist.
- Walkthroughs explain the **thought process** (GDB analysis, vulnerability identification, exploit crafting) rather than just the final answer.

---

## 📚 Resources

- [GDB Cheatsheet](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)
- [pwntools Documentation](https://docs.pwntools.com/)
- [exploit.education](https://exploit.education/) — similar exercises for extra practice
- [LiveOverflow YouTube](https://www.youtube.com/@LiveOverflow) — binary exploitation tutorials

---

## ⚖️ Disclaimer

This repository is for **educational purposes only**. The techniques demonstrated are studied in the context of the 42 School curriculum to build an understanding of low-level security. Do not apply these techniques to systems you do not own or have explicit permission to test.

---

## 👤 Author

| Login | School |
|-------|--------|
| your_login | 42 |
