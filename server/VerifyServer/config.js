const fs = require('fs');
const path = require('path');

const configPath = path.join(__dirname, 'config.json');
let config = JSON.parse(fs.readFileSync(configPath, 'utf8'));

if (!config) {
    console.error('Failed to load configuration from config.json');
    process.exit(1);
}

// Extracting configuration values
let email_user = config.email.user;
let email_password = config.email.password;

let mysql_host = config.mysql.host;
let mysql_port = config.mysql.port;
let mysql_password = config.mysql.password;

let redis_host = config.redis.host;
let redis_port = config.redis.port;
let redis_password = config.redis.password;

module.exports = {
    email_user,
    email_password,
    mysql_host,
    mysql_port,
    mysql_password,
    redis_host,
    redis_port,
    redis_password
}