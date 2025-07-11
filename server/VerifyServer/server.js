const grpc = require('@grpc/grpc-js');
const message_proto = require('./proto.js');
const const_module = require('./const.js');
const email_module = require('./email.js');
const config = require('./config.js');
const redis_module = require('./redis.js');

async function GetVerifyCode(call, callback) {
    console.log("email is: ", call.request.email);
    try {
    // Check if the email already exists in Redis
    let key = const_module.code_prefix + call.request.email;
    let exists = await redis_module.exists(key);
    let code = null;
    if (exists) {
        console.log("Email already exists in Redis, returning existing code.");
        code = await redis_module.get(key);
    } else {
        console.log("Email does not exist in Redis, generating new code.");
        code = Math.floor(Math.random() * 900000) + 100000;
        // Set the code in Redis with an expiration time of 5 minutes
        let setResult = await redis_module.setExpire(key, code.toString(), 300);
        if (!setResult) {
            console.error("Failed to set code in Redis.");
            return callback(null, {
                error: const_module.Errors.REDISERR,
                email: call.request.email,
                code: null
            });
        }
        console.log("New code generated and set in Redis: ", code);
    }

    let mailOptions = {
        from: config.email_user, // sender address
        to: call.request.email,
        subject: '【注册验证】',
        text: `您的验证码是: ${code}`,
        html: `<h3>您的验证码是：<b>${code}</b></h3>`
    }
    let send_result = await email_module.SendEMail(mailOptions);
    console.log("send_result: ", send_result);

    callback(null, { // 把响应数据返回给客户端-> 返回GetVerifyResponse
        error: const_module.Errors.SUCCESS,
        email: call.request.email,
        code: code // to string
    });

    } catch (error) {
        console.error("Error occurred while sending email: ", error);
        await redis_module.del(key); // 清除 Redis 中的验证码
        callback(null, {
            error: const_module.Errors.EXCEPTION,
            email: call.request.email,
            code: null
        });
    }
}

function main() {
    let server = new grpc.Server();
    server.addService(message_proto.VerifyService.service, {
        GetVerifyCode: GetVerifyCode  // protobuf定义的服务方法: local fake GetVerifyCode
    });
    server.bindAsync('localhost:50051', grpc.ServerCredentials.createInsecure(), (error) => {
        if (error) {
            console.error("Failed to bind server: ", error);
            return;
        }
        console.log(`Server running at http://localhost:50051`);
    });
}

main();