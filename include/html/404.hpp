/*****************************************************************************
 *
 * Future House Technologies
 *
 * Copyright (C) 2022 - Denisov Foundation Limited
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 *****************************************************************************/

#ifndef __404_HTML_HPP__
#define __404_HTML_HPP__

#include <Arduino.h>

String PROGMEM notFoundHtml = "\
    <!DOCTYPE html> \
    <html> \
        <body> \
            <center><h1>404 Not Found</h1></center> \
        </body> \
    </html>";

#endif /* __404_HTML_HPP__ */
