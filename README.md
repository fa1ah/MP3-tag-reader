# MP3-tag-reader

A lightweight command-line tool written in C for viewing and editing ID3v2 tags in MP3 files. Easily manage metadata like title, artist, album, year, genre, and comments directly from your terminal.

## Features

- **View Tags**: Display all ID3v2 metadata from MP3 files
- **Edit Tags**: Modify individual tags (Title, Artist, Album, Year, Genre, Comments)
- **ID3v2 Support**: Works with ID3v2 standard tags
- **File Validation**: Automatic MP3 file format verification
- **Year Validation**: Ensures year values are within valid range (1600-2100)
- **In-place Editing**: Safely replaces original file with edited version

## Supported Tags

| Tag Specifier | Tag Name | ID3v2 Frame |
|---------------|----------|-------------|
| `-T` | Title | TIT2 |
| `-A` | Artist | TPE1 |
| `-a` | Album | TALB |
| `-Y` | Year | TYER |
| `-C` | Content Type (Genre) | TCON |
| `-c` | Comments | COMM |


## Usage

### View Tags
Display all ID3v2 tags from an MP3 file:

```bash
./mp3tag -v sample.mp3
```

**Output:**
```
——————————————————————————————————————————————————————————————————————————
MP3 Tag Reader & Editor for ID3v2
——————————————————————————————————————————————————————————————————————————
Title                : My Favorite Song
Artist               : Great Artist
Album                : Best Album
Year                 : 2023
Content Type         : Pop
Composer             : Amazing composer
——————————————————————————————————————————————————————————————————————————
```

### Edit Tags
Modify a specific tag in an MP3 file:

```bash
# Change title
./mp3tag -e -T "New Song Title" sample.mp3

# Change artist
./mp3tag -e -A "New Artist Name" sample.mp3

# Change album
./mp3tag -e -a "New Album" sample.mp3

# Change year
./mp3tag -e -Y "2024" sample.mp3

# Change genre
./mp3tag -e -C "Rock" sample.mp3

# Change comments
./mp3tag -e -c "New comment" sample.mp3
```

### Help Menu
Display usage information:

```bash
./mp3tag -h
```

## Command Syntax

```
./mp3tag [OPTION] [ARGS...]

Options:
  -v <filename>                          View ID3v2 tags
  -e <specifier> <new_value> <filename>  Edit a specific tag
  -h                                     Display help menu
```

## How It Works

1. **Validation**: Checks if the file is a valid MP3 with ID3v2 tags
2. **Tag Reading**: Parses ID3v2 frames starting after the 10-byte header
3. **Endian Conversion**: Handles big-endian to little-endian conversion for frame sizes
4. **Tag Editing**: Creates a new file with updated metadata and replaces the original

## Project Structure

```
MP3-id3v2-editor/
│
├── main.c           # Main program and command-line argument handling
├── tag_editor.c     # Core functions for reading and editing tags
├── tag_editor.h     # Header file with function declarations
└── README.md        # This file
```

## Technical Details

- **ID3v2 Frame Format**: Reads 4-byte frame IDs, 4-byte sizes, and 2-byte flags
- **Text Encoding**: Handles text encoding identifiers in frame data
- **Binary File Operations**: Uses binary read/write to preserve MP3 audio data
- **Memory Management**: Efficient file pointer manipulation and dynamic reading

## Limitations

- Only supports ID3v2 tags (not ID3v1)
- Edits are performed by file replacement (creates temporary file)
- Limited to 6 predefined tags
- Maximum 81 tag iterations during search

## Error Handling

The program handles:
- Invalid file access
- Non-MP3 files
- Invalid edit specifiers
- Out-of-range year values (must be 1600-2100)
- Missing tags in MP3 files

## Examples

```bash
# View tags
./mp3tag -v mysong.mp3

# Change multiple tags
./mp3tag -e -T "Bohemian Rhapsody" mysong.mp3
./mp3tag -e -A "Queen" mysong.mp3
./mp3tag -e -Y "1975" mysong.mp3
```


## Author

Falah Nizar
