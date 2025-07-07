const nodemailer = require('nodemailer');
const config = require('./config.js');

// Create a transporter object using SMTP transport
const transporter = nodemailer.createTransport({
    host: 'smtp.qq.com', // SMTP server
    port: 465, // SMTP port
    secure: true, // true for 465, false for other ports
    auth: {
        user: config.email_user, // sender email address
        pass: config.email_password // sender email password
    }
});

// Function to send an email
/*
- `resolve`：表示异步成功时调用；
- `reject`：表示异步失败时调用。
`info.response` 通常包含邮件服务器返回的响应信息，比如：`250 OK` 或 `Queued as xyz123`。
*/
function SendEmail(mailOptions_) {
    // Send mail with defined transport object
    return new Promise(function(resolve, reject) {
        transporter.sendMail(mailOptions_, function(error, info) { // info contains response from the server
            if (error) {
                reject(error); // Reject the promise with the error, equivalent to throwing an error and returning
            } else {
                resolve(info.response); // Resolve the promise with the response, equivalent to returning a value
            }
        });
    });
}

// Export the SendEmail function
module.exports.SendEMail = SendEmail;