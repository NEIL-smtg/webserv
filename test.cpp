#include <iostream>

std::string generateErrorHtmlPage(int error_code, std::string error_message)
{
    return ('<!DOCTYPE html><html><head><meta charset="UTF-8"><title>ERROR PAGE</title><style>.c {text-align: center;margin-top: 100px;}h1 {font-size: 72px;}p {font-size: 24px;}</style></head><body><div class="c"><h1>' + error_code + "-" + error_message + "</h1><p>Oops! The page you're looking for doesn't exist.</p></div></body>");
}
