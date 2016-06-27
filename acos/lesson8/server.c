#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <netinet/in.h>

int main()
{
	struct sockaddr_un addr;
	int s = socket(PF_LOCAL, SOCK_STREAM, IPPROTO_IP);
	addr.sun_family = AF_LOCAL;
	strcpy(addr.sun_path, "/tmp/test");
	bind(s, (struct sockaddr*)&addr, sizeof(addr));
	listen(s, 10);
	int d = accept(s, NULL, NULL);
	return 0;
}
