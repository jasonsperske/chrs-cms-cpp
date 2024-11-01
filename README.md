# chrs-cms-cpp

An AI powered CPP port of the chrs-cms-retro project

# Library CMS with OpenAI Vision Integration

A C++ web application built with the Drogon framework that provides a Content Management System (CMS) for managing a library catalog with AI-powered book/magazine recognition capabilities.

## Features

- RESTful API endpoints for managing library entries
- OpenAI Vision API integration for automatic book/magazine metadata extraction
- Image processing with automatic resizing and optimization
- SQLite database for data persistence
- JSON-based API responses
- Web-based user interface

## Prerequisites

Before you begin, ensure you have the following installed on your system:

- C++ compiler with C++17 support (GCC 7+ or Clang 5+)
- CMake (3.14 or higher)
- OpenCV (4.0 or higher)
- SQLite3
- JsonCpp
- Drogon framework

### System Dependencies (Ubuntu/Debian)

```bash
# Install system dependencies
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libsqlite3-dev \
    libjsoncpp-dev \
    libopencv-dev \
    uuid-dev \
    zlib1g-dev

# Install Drogon
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

### System Dependencies (macOS)

```bash
# Install dependencies using Homebrew
brew install \
    cmake \
    openssl \
    sqlite \
    jsoncpp \
    opencv \
    ossp-uuid \
    drogon
```

## Project Setup

1. Clone the repository:

```bash
git clone https://github.com/jasonsperske/chrs-cms-cpp
cd chrs-cms-cpp
```

2. Create a `.env` file in the project root:

```bash
OPENAI_API_KEY=your_openai_api_key_here
```

3. Build the project:

```bash
mkdir build
cd build
cmake ..
make
```

4. Initialize the database:

```bash
sqlite3 library.sqlite < ../sql/schema.sql
```

5. Run the application:

```bash
./library-cms
```

The application will be available at `http://localhost:3000`

## Project Structure

```
library-cms/
├── CMakeLists.txt
├── README.md
├── .env
├── src/
│   ├── main.cpp
│   ├── controllers/
│   │   ├── EntryController.cpp
│   │   ├── EntryController.h
│   │   ├── VisionController.cpp
│   │   └── VisionController.h
│   └── models/
│       ├── Entry.cpp
│       ├── Entry.h
│       ├── Library.cpp
│       └── Library.h
├── templates/
│   ├── layout.csp
│   └── index.csp
└── sql/
    └── schema.sql
```

## API Endpoints

### GET /

Returns the main web interface for the library CMS.

### GET /entry/{id}.json

Returns metadata for a specific library entry.

**Response Format:**

```json
{
  "success": true,
  "response": {
    "id": 1,
    "title": "Book Title",
    "author": "Author Name"
    // ... additional metadata
  }
}
```

### POST /api/openai/vision

Analyzes images of books or magazines using OpenAI's Vision API to extract metadata.

**Request Body:**

- `files`: Array of image files (multipart/form-data)

**Response Format:**

```json
{
  "success": true,
  "response": {
    "mediaType": "book",
    "title": "Detected Title",
    "author": "Detected Author"
    // ... additional metadata
  }
}
```

## Database Schema

The application uses SQLite with the following basic schema (expand as needed):

```sql
CREATE TABLE IF NOT EXISTS library (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        mediaType TEXT NOT NULL,
        title TEXT NOT NULL,
        author TEXT,
        publishedBy TEXT,
        publishedLocation TEXT,
        edition TEXT,
        editionYear TEXT,
        publishedOn TEXT,
        serialNumber TEXT,
        catalogNumber TEXT,
        section TEXT
      );
```

## Development

### Adding New Controllers

1. Create header and implementation files in `src/controllers/`
2. Register the new controller in `main.cpp`
3. Update CMakeLists.txt if needed

### Adding New Models

1. Create header and implementation files in `src/models/`
2. Update the database schema if needed
3. Update CMakeLists.txt if needed

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [Drogon Framework](https://github.com/drogonframework/drogon)
- [OpenAI API](https://openai.com/api/)
- [OpenCV](https://opencv.org/)
