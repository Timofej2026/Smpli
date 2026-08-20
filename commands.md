# smpli Language Commands & Data Types Documentation

This is the full reference guide for the **smpli** programming language syntax, built-in functions, and data types.

## Core Rules
- Brackets `()` must contain text, variables, or expressions.
- Curly braces `{}` define block structures, conditions, and logical scope.
- The `print()` command functions identically to Python's print implementation.

---

## Script Lifecycle & Variables

| Command | Description |
| :--- | :--- |
| `start` | Marks the absolute beginning of the script execution. |
| `stop` | Terminates script execution immediately. |
| `scriptTag()` | Defines script metadata or internal identifiers. |
| `use()` | Imports an external module, framework, or graphic engine. |
| `numVar=()` | Allocates memory and stores a numeric integer value. |
| `var()` | Stores a single alphabetical letter or character. |
| `word()` | Allocates memory and stores a string literal text. |
| `()±()` | Handles complex mathematical evaluations and logic equations. |
| `()=()` | Defines or reassigns values directly. |
| `±()=()` | Evaluates a dynamic expression involving variables. |

## Logic, Flow Control & Events

| Command | Description |
| :--- | :--- |
| `if` | Initiates a basic conditional check block. |
| `then` | Sets the block execution path if the conditional check is met. |
| `else` | Sets an alternative execution path if conditions fail. |
| `do` | Explicitly triggers an inline operation block. |
| `toDo()` | Custom loop or specific sequence action call. |
| `check()` | Runs a runtime verification or assertion on data state. |
| `whenPressed()Do()` | Listens for a hardware keypress event and fires a routine. |

## Messaging & Notifications

| Command | Description |
| :--- | :--- |
| `sendMessage()To()` | Dispatches an internal system message packet to a target object. |
| `givenMessage()` | Captures or intercepts the last received communication string. |
| `alert()` | Renders an instantaneous pop-up or notification window. |
| `alertSound()` | Triggers a hardware-level audio buzzer alert. |

## File System & Data Management

| Command | Description |
| :--- | :--- |
| `data` | Opens a stream for writing block data lines. |
| `readData()` | Opens, reads, and dumps file content strings. |
| `deleteData()` | Unlinks and completely destroys specified file paths. |
| `moveData()` | Transports or renames standard file assets on storage. |

## Rendering & Advanced Formatting

| Command | Description |
| :--- | :--- |
| `printHex()` | Converts integers and prints format to base-16 Hexadecimal. |
| `printBinary()` | Converts integers and prints format to base-2 Binary. |
| `dataType()` | Sets data context constraints using valid engine data types. |
| `addDataType()` | Extends engine core runtime with a brand new custom type descriptor. |
| `putText()` | Draws an alphanumeric text asset at predefined vector targets. |
| `putNum()` | Draws integer data objects at predefined vector targets. |
| `hint()` | Creates a hovering helper element descriptor for display elements. |

---

## Supported Data Types

These parameters can be passed into the `dataType()` function to specify what kind of asset or layout the script is currently manipulating.

### Graphics & Engine Architecture
- `3Dmodel` — Low-level 3D mesh geometry data.
- `2DSprite` — Standard 2D bitmap flat graphic asset.
- `3Dprefab` — Pre-configured 3D template object structure.
- `2Dprefab` — Pre-configured 2D template layout structure.
- `3Dscene` — Global container environment map for 3D worlds.
- `2Dscene` — Global container environment map for 2D worlds.

### Scripts & Documents
- `textDocument` — Plain text files and configuration documents.
- `script` — Standard raw instruction files.
- `pythonScript` — Embedded backend scripts executed via Python engine.

### Visual Media Formats
- `picturePNG` — Portable Network Graphics alpha-channel format image.
- `pictureJPG` — Joint Photographic Experts Group raster standard image.
- `videoMP4` — Standard digital multimedia container for video streams.
- `videoMOV` — Apple QuickTime multimedia file standard container.

### Audio Assets
- `musicMP3` — Compressed digital audio layer encoding format.
- `musicOGG` — Open-source Ogg Vorbis compressed audio stream.
- 
