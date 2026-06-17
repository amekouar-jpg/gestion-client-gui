# 📚 Documentation Index

Welcome! This file helps you navigate the GTK3 Contact Manager project.

## 🎯 Choose Your Path

### 👤 I'm a Regular User
**Goal**: Build and use the contact manager

**Reading Order**:
1. [README.md](README.md) - Features and user guide (10 min read)
2. [BUILD.md](BUILD.md) - Build instructions for your OS (5 min read)
3. Run the application!

**Key Topics**:
- How to add/modify/delete contacts
- How to search and save files
- Requirements and installation

---

### 👨‍💻 I'm a Developer / Student
**Goal**: Understand the code and make modifications

**Reading Order**:
1. [README.md](README.md) - Overview (5 min)
2. [BUILD.md](BUILD.md) - Build on your platform (5 min)
3. [DEVELOPER.md](DEVELOPER.md) - How to modify code (15 min)
4. [ARCHITECTURE.md](ARCHITECTURE.md) - Deep technical dive (20 min)

**Key Topics**:
- How to add new features
- Memory management patterns
- GTK3 signal handling
- Data structure design

---

### 🏗️ I'm Learning Software Architecture
**Goal**: Understand design patterns and best practices

**Reading Order**:
1. [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) - Overview (5 min)
2. [ARCHITECTURE.md](ARCHITECTURE.md) - Design patterns (25 min)
3. [projet_gui.c](projet_gui.c) - Read the source (30 min)
4. [DEVELOPER.md](DEVELOPER.md) - Extension examples (15 min)

**Key Topics**:
- Model-View-Controller pattern
- Memory safety in C
- GTK3 callback architecture
- Signal handling and event loops

---

### 🔧 I Want to Extend the Application
**Goal**: Add new features

**Quick Start**:
1. [BUILD.md](BUILD.md) - Get it running (5 min)
2. [DEVELOPER.md](DEVELOPER.md) - Common modifications section (10 min)
3. Try a modification
4. [ARCHITECTURE.md](ARCHITECTURE.md) - For complex changes (20 min)

**Example Features to Add**:
- New columns to the contacts table
- Export to CSV format
- Statistics window
- Advanced search filters
- Dark mode toggle

---

### 🐛 Something Isn't Working
**Goal**: Troubleshoot and fix issues

**Quick Reference**:
1. [BUILD.md](BUILD.md) - "Troubleshooting Steps" section
2. [DEVELOPER.md](DEVELOPER.md) - "Debug Tips" and "Common Bugs & Fixes"
3. Check error messages in dialogs

**Common Issues**:
- Compilation errors → Check BUILD.md prerequisites
- Memory errors → See ARCHITECTURE.md memory management
- Runtime crashes → See DEVELOPER.md debug tips

---

## 📄 File Directory

### Source Code
```
projet_gui.c              Main GUI application (650 lines, production-ready)
```

### Build Configuration
```
Makefile                 Linux/macOS build (recommended)
CMakeLists.txt          CMake build (alternative)
build.bat               Windows batch build script
```

### Documentation
```
README.md               User manual and feature guide
BUILD.md                Detailed platform-specific build instructions
ARCHITECTURE.md         Technical architecture and design patterns
DEVELOPER.md            Quick reference for code modifications
PROJECT_SUMMARY.md      High-level project overview
INDEX.md                This file (navigation guide)
```

### Data Files (created at runtime)
```
pers.bin                Binary contact database (created on save)
pers.txt                Text contact database (created on save)
```

---

## 🗂️ Documentation by Topic

### Getting Started
- [README.md - Features](README.md#features)
- [README.md - Installation](README.md#installation)
- [BUILD.md - Quick Start](BUILD.md#quick-start)

### Building & Compilation
- [BUILD.md - Linux](BUILD.md#1-linux-ubuntudebian)
- [BUILD.md - macOS](BUILD.md#2-macos-intelapple-silicon)
- [BUILD.md - Windows](BUILD.md#4-windows-msys2--mingw64)
- [BUILD.md - Troubleshooting](BUILD.md#troubleshooting)

### Using the Application
- [README.md - User Guide](README.md#user-guide)
- [README.md - Operations](README.md#operations)
- [README.md - File Formats](README.md#data-file-formats)

### Code Modifications
- [DEVELOPER.md - Common Modifications](DEVELOPER.md#common-modifications)
- [DEVELOPER.md - Adding Features](DEVELOPER.md#adding-new-features)
- [DEVELOPER.md - Debug Tips](DEVELOPER.md#debug-tips)

### Technical Details
- [ARCHITECTURE.md - Overview](ARCHITECTURE.md#overview)
- [ARCHITECTURE.md - Memory Management](ARCHITECTURE.md#memory-management-strategy)
- [ARCHITECTURE.md - Data Persistence](ARCHITECTURE.md#data-persistence)

---

## 🚀 Quick Links

### I Want To...

**...build the application**
→ [BUILD.md - Quick Start](BUILD.md#quick-start)

**...understand the features**
→ [README.md - Features](README.md#features)

**...learn how the code works**
→ [ARCHITECTURE.md - Core Components](ARCHITECTURE.md#core-components)

**...add a new feature**
→ [DEVELOPER.md - Common Modifications](DEVELOPER.md#common-modifications)

**...fix a compilation error**
→ [BUILD.md - Common Build Errors](BUILD.md#common-build-errors--solutions)

**...understand memory management**
→ [ARCHITECTURE.md - Memory Management](ARCHITECTURE.md#memory-management-strategy)

**...learn GTK3 patterns**
→ [ARCHITECTURE.md - Event-Driven Architecture](ARCHITECTURE.md#event-driven-architecture)

**...debug a crash**
→ [DEVELOPER.md - Debug Tips](DEVELOPER.md#debug-tips)

**...see the project status**
→ [PROJECT_SUMMARY.md - Completion Checklist](PROJECT_SUMMARY.md#-completion-checklist)

---

## 📊 Content Overview

| Document | Length | Audience | Focus |
|----------|--------|----------|-------|
| README.md | 3 min | Everyone | What & How to Use |
| BUILD.md | 5 min | Developers | Building & Setup |
| ARCHITECTURE.md | 20 min | Developers | Technical Design |
| DEVELOPER.md | 10 min | Developers | Code Modification |
| PROJECT_SUMMARY.md | 5 min | Everyone | Overview & Stats |

---

## 🎓 Learning Path (Recommended)

### Beginner (Just Want to Use It)
1. [README.md](README.md) - Read all
2. [BUILD.md - Quick Start](BUILD.md#quick-start) - Build
3. Use the application!

**Time**: 15 minutes

### Intermediate (Want to Modify It)
1. [README.md](README.md) - Full read
2. [BUILD.md](BUILD.md) - Full read
3. [DEVELOPER.md - Modifications](DEVELOPER.md#common-modifications) - Try one
4. [ARCHITECTURE.md - Components](ARCHITECTURE.md#core-components) - Understand structure

**Time**: 1 hour

### Advanced (Want to Master It)
1. All documentation (README, BUILD, DEVELOPER, ARCHITECTURE)
2. Read entire [projet_gui.c](projet_gui.c) source
3. Implement new features from [DEVELOPER.md - Adding Features](DEVELOPER.md#adding-new-features)
4. Study [ARCHITECTURE.md - Design Patterns](ARCHITECTURE.md#design-patterns-used)

**Time**: 3-4 hours

### Expert (Want to Extend It)
1. Complete mastery of all above
2. Implement complex extensions (async I/O, sorting, filtering)
3. Write additional documentation
4. Package for distribution

**Time**: Full project integration

---

## 🔍 Search by Keyword

### Memory Management
- [ARCHITECTURE.md - Memory Management Strategy](ARCHITECTURE.md#memory-management-strategy)
- [DEVELOPER.md - Bug: Memory Leak on Error](DEVELOPER.md#bug-memory-leak-on-error)

### GTK3 & GUI
- [ARCHITECTURE.md - UI Component Hierarchy](ARCHITECTURE.md#ui-component-hierarchy)
- [ARCHITECTURE.md - Event-Driven Architecture](ARCHITECTURE.md#event-driven-architecture)
- [DEVELOPER.md - GTK Reference](DEVELOPER.md#gtk-reference-quick-links)

### File Operations
- [README.md - File Formats](README.md#data-file-formats)
- [ARCHITECTURE.md - Data Persistence](ARCHITECTURE.md#data-persistence)

### Performance
- [ARCHITECTURE.md - Performance Analysis](ARCHITECTURE.md#performance-analysis)
- [DEVELOPER.md - Performance Optimization](DEVELOPER.md#performance-optimization)

### Error Handling
- [ARCHITECTURE.md - Error Handling](ARCHITECTURE.md#error-handling-strategy)
- [BUILD.md - Troubleshooting](BUILD.md#troubleshooting)
- [DEVELOPER.md - Common Bugs](DEVELOPER.md#common-bugs--fixes)

---

## 💡 Tips & Tricks

### Pro Tips
- Use `make clean && make` for clean builds
- Read [DEVELOPER.md](DEVELOPER.md) for quick code reference
- Bookmark [ARCHITECTURE.md](ARCHITECTURE.md) for technical questions
- Check [BUILD.md](BUILD.md) before reporting build issues

### Time-Saving
- Already familiar with GTK? Jump to [DEVELOPER.md](DEVELOPER.md)
- Just want to compile? Go to [BUILD.md - Quick Start](BUILD.md#quick-start)
- Curious about internals? Read [ARCHITECTURE.md](ARCHITECTURE.md)

### First-Time Setup
1. Read [README.md](README.md) intro
2. Follow [BUILD.md](BUILD.md) for your OS
3. Run application
4. Test features from [README.md - Operations](README.md#operations)

---

## ❓ FAQ

**Q: Where do I start?**  
A: If you just want to use it → [README.md](README.md)  
If you want to develop → [BUILD.md](BUILD.md) then [DEVELOPER.md](DEVELOPER.md)

**Q: How do I build on Windows?**  
A: See [BUILD.md - Windows](BUILD.md#4-windows-msys2--mingw64)

**Q: How do I add a new feature?**  
A: Start with [DEVELOPER.md - Common Modifications](DEVELOPER.md#common-modifications)

**Q: Is there a memory leak?**  
A: No, see [ARCHITECTURE.md - Memory Safety Guarantees](ARCHITECTURE.md#memory-safety-guarantees)

**Q: Can I modify and redistribute?**  
A: Yes, educational use - see [PROJECT_SUMMARY.md - License](PROJECT_SUMMARY.md#license--attribution)

**Q: What GTK version is required?**  
A: GTK 3.0 or later (any version)

**Q: Is there a tutorial?**  
A: See [README.md - User Guide](README.md#user-guide) and [DEVELOPER.md](DEVELOPER.md)

---

## 📞 Still Need Help?

### Issue Type → Solution

**Build fails** → [BUILD.md - Common Errors](BUILD.md#common-build-errors--solutions)  
**Application crashes** → [DEVELOPER.md - Debug Tips](DEVELOPER.md#debug-tips)  
**Want to add feature** → [DEVELOPER.md - Modifications](DEVELOPER.md#common-modifications)  
**Understand memory** → [ARCHITECTURE.md - Memory](ARCHITECTURE.md#memory-management-strategy)  
**GTK question** → [DEVELOPER.md - GTK Reference](DEVELOPER.md#gtk-reference-quick-links)  
**Need overview** → [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)

---

## 📖 Document Relationships

```
PROJECT_SUMMARY.md
    ├─→ README.md (what it does)
    ├─→ BUILD.md (how to build)
    └─→ [ARCHITECTURE.md + DEVELOPER.md] (how it works)

README.md
    └─→ BUILD.md (detailed platform help)

DEVELOPER.md
    └─→ ARCHITECTURE.md (deeper understanding)

ARCHITECTURE.md
    └─→ projet_gui.c (source code)
```

---

**🎉 You're ready to go!**

Pick your path above and start exploring.

**Recommended first step**: 
- Users → [README.md](README.md)
- Developers → [BUILD.md](BUILD.md)
