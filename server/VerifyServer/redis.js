const config_module = require('./config.js');
const redis = require('ioredis');

// redis client
const redisClient = new redis({
    host: config_module.redis_host,
    port: config_module.redis_port,
    password: config_module.redis_password
});

// 连接成功监听
redisClient.on('connect', () => {
    console.log('Redis client connected');
});

// listen for errors
redisClient.on('error', (err) => {
    console.error('Redis error:', err);
});

// get function
async function get(key) {
    try {
        const value = await redisClient.get(key);
        if (value === null) {
            console.warn(`Key "${key}" does not exist in Redis.`);
            return null; // Key does not exist
        }
        return value; // Return the value as a string
    } catch (err) {
        console.error('Error getting key from Redis:', err);
        throw err; // Re-throw the error for handling by the caller
    }
}

// exists function
async function exists(key) {
    try {
        const exists = await redisClient.exists(key);
        return exists !== 0;
    } catch (err) {
        console.error('Error checking key existence in Redis:', err);
        throw err; // Re-throw the error for handling by the caller
    }
}

// setExpire function
async function setExpire(key, value, expireTime) {
    try {
        const result = await redisClient.set(key, value, 'EX', expireTime);
        if (result !== 'OK') {
            console.error(`Redis SET command failed with result: ${result}`);
            return false;
        }
        return true;
    } catch (err) {
        console.error('Error setting key expiration in Redis:', err);
        throw err; // Re-throw the error for handling by the caller
    }
}

// quit 函数
async function quit() {
    try {
        await redisClient.quit();
        console.log('Redis client disconnected');
    } catch (err) {
        console.error('Error disconnecting Redis client:', err);
    }
}

// Export the functions
module.exports = {
    get,
    exists,
    setExpire,
    quit
};