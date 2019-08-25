// VulkanTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <assert.h>
#include "vkapp.h"


int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE preInstance, LPWSTR lpCmdLine, int lcmdShow) {
    glfwInit( );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    GLFWwindow* window = glfwCreateWindow( 400, 300, "VulkanTest", nullptr, nullptr );

    vkapp vkapp;
    vkapp.init(hinstance, window);
    vkapp.enumLayerProperties( );
    vkapp.enumExtensionProperties( );
    vkapp.finish( );
    return 0;
}
