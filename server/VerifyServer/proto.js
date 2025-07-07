const path = require('path');
const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');

const PROTO_PATH = path.join(__dirname, 'message.proto');

const packageDefinition = protoLoader.loadSync(PROTO_PATH, { // Load the proto file
    keepCase: true, // Keep field names in camelCase
    longs: String, // Convert long integers to strings
    defaults: true, // Use defaults for missing fields
    oneofs: true // Use oneofs for better compatibility
});

const message_proto = grpc.loadPackageDefinition(packageDefinition).message; // Load the package definition into a gRPC object

module.exports = message_proto;
