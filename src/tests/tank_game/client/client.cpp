#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "Camera.h"
#include "DebugRenderer.h"
#include "Platform.h"
#include <glm/glm.hpp>

#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <string>

#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "Api.h"
#include "TankMap.h"

void setNonblock(int sock);

void setupGui();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


int32_t gWidth = 640;
int32_t gHeight = 480;
GLFWwindow *gWindow = NULL;

int sock;

void cleanup()
{
	close(sock);
}

int main ( int argc, char** argv )
{
	if(argc != 2)
	{
		printf("dynamic_map ip\n");
		return 0;
	}

	char* host = argv[1];
	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock==-1)
	{
		printf("socket errror\n");
		return 0;
	}

	atexit(cleanup);
	
	struct sockaddr_in sa;
   	bzero(&sa,sizeof(sa));
   	sa.sin_family      = AF_INET;
   	sa.sin_addr.s_addr = inet_addr(host);
   	sa.sin_port        = htons(31415);

	while(connect(sock, (struct sockaddr *)&sa, sizeof(sa)) ==-1)
	{
		printf("failed to connect...\n");
		usleep(100000);
	}

	char message[message_max_size];

	// generate the world
	TankMap tm;

	bool mapValid = false;

	int width = 0;
	int myId = -1;
	int myPos = -1;

	for(;;)
	{
		printf("connected and waiting for server...\n");
		int rc = recv(sock, message, message_max_size, 0);
		if(rc == -1)
		{
			printf("failed to receive...\n");
			return 0;
		}
		else if(rc==0)
		{
			printf("bad.\n");
			return 0;
		}
		else if (rc >= sizeof(HeresMapMsg) && ((HeresMapMsg*)message)->hdr.type == HeresMap)
		{
			HeresMapMsg* hmm = (HeresMapMsg*)message;
			if(hmm->hdr.type == HeresMap)
			{
				printf("got map...\n");
				tm.set((float*)(((char*)hmm) + sizeof(HeresMapMsg)), hmm->width);
				
				width = hmm->width;
				myId = hmm->youare;
				if(myId==0)
				{
					myPos = hmm->player_1_pos;
				}
				else 
				{
					myPos = hmm->player_2_pos;
				}

				mapValid = true;
				break;
			}
		}
	}

	if(!mapValid)
	{
		printf("got invalid map.\n");
		exit(0);
	}

	setupGui();

	// set up rendering
    std::shared_ptr<DebugRenderer> worldRenderer(new DebugRenderer());

    std::shared_ptr<Camera> camera (new Camera());
    camera->setView(glm::vec3(gWidth/2.f, gHeight/2.f, 0.0),
                              glm::vec3(gWidth/2.f, gHeight/2.f, -1.0),
                              glm::vec3(0.0, 1.0, 0.0));
    camera->setOrthographic(-gWidth/2.f, gWidth/2.f, -gHeight/2.f, gHeight/2.f, -1, 1);

    float stepX = (float)gWidth / (float)width;
    for(int ii=1; ii<width; ++ii)
    {
    	std::vector<glm::vec3> points;
    	points.push_back(glm::vec3((float)(ii-1)*stepX, tm.getHeight(ii-1), 0.1f));
    	points.push_back(glm::vec3((float)ii*stepX, tm.getHeight(ii), 0.1f));
    	worldRenderer->addLines(points, glm::vec3(1.0f, 1.0f, 1.0f));
    	//fprintf(stderr, "(%f, %f)\n", (float)ii*stepX, tm.getHeight(ii));
    }

    setNonblock(sock);

    while (!glfwWindowShouldClose(gWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        worldRenderer->draw(camera);

		int rc = recv(sock, message, message_max_size, 0);
		if(rc == -1 && errno!=EWOULDBLOCK && errno!=EAGAIN)
		{
			perror("failed to receive...\n");
			break;
		}
		else if(rc>0)
		{
			printf("got message...\n");
		}

        glfwSwapBuffers(gWindow);
        glfwPollEvents();
	}

	close(sock);

    glfwDestroyWindow(gWindow);
    glfwTerminate();

	return 0;
}

void setupGui()
{
	if (!glfwInit())
    {
        exit(1);
    }

    // make the window
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    gWindow = glfwCreateWindow(gWidth, gHeight, "Map Rendering", NULL, NULL);
    if (!gWindow)
    {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(gWindow);
    glfwSetKeyCallback(gWindow, key_callback);
    glewInit();

    glViewport(0, 0, gWidth, gHeight);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0., 0., 0.112f, 1.);
    glClearDepth(1.0);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void setNonblock(int sock)
{
	int flags = fcntl(sock, F_GETFL, 0);
   	if (flags < 0)
   	{
   		printf("fcntl failed\n");
   	}
   	else {
   		flags |= O_NONBLOCK;
		fcntl(sock, F_SETFL, flags);
   	}
}