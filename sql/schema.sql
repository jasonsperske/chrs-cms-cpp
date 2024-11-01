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