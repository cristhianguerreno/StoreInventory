USE Proyecto;
GRANT ALL PRIVILEGES ON inventory_db.* TO 'userproyecto'@'localhost';
FLUSH PRIVILEGES;
DROP TABLE IF EXISTS products;

CREATE TABLE products (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    quantity INT NOT NULL,
    image_path VARCHAR(500),
    image_data LONGBLOB,
    brand VARCHAR(255),
    size INT,
    category VARCHAR(255),
    deposit VARCHAR(255),
    minimum_stock INT DEFAULT 5,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
