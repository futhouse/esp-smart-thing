R"=====(
    <!DOCTYPE html>
    <html>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <head>
            <center>
                <h1>ESP Smart Thing</h1>
                <table>
                    <tr>
                        <td><a href='/'>Info</a></td>
                        <td><a href='/wifi.html'>Wi-Fi</a></td>
)====="
#ifdef SMS_NOTIFY_MOD
R"=====(
                        <td><a href='/sms.html'>SMS</a></td>
)====="
#endif
#ifdef TELEGRAM_NOTIFY_MOD
R"=====(
                        <td><a href='/telegram.html'>Telegram</a></td>
)====="
#endif
#ifdef SECURE_MOD
R"=====(
                        <td><a href='/secure.html'>Secure</a></td>
)====="
#endif
R"=====(
                    </tr>
                </table>
            </center>
        </head>
        <hr>
        <style type="text/css">
            * { font-family: arial; }
        </style>
)====="