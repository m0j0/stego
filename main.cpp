#include <stdio.h>
#include "jpge.h"
#include "jpgd.h"
#include "crypto.h"

#include "stb_image.c"

int main(int argc, char *argv[])
{
	const int req_comps = 3;
	int width = 0, height = 0, actual_comps = 0;
	
	uint8 *pImage_data = stbi_load("_test_.jpg", &width, &height, &actual_comps, req_comps);
  
	if (!pImage_data)
	{
		printf("Failed loading file \"%s\"!\n", "test.jpg");
		getchar();
		return EXIT_FAILURE;
	}
	
	jpge::params params;
	params.m_quality = 100;
	params.m_subsampling = jpge::H1V1;
	params.m_two_pass_flag = 0;

	stegosystem stparams;
	stparams.stego_data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse nec velit enim. Sed cursus ultrices orci, sit amet imperdiet libero luctus eget. Cras ut purus vitae odio consectetur dignissim a ut enim. Sed sit amet erat tortor. Aenean nunc eros, eleifend quis varius non, sollicitudin eu augue. Curabitur mollis at ligula lobortis placerat. Vestibulum eget massa urna. Praesent ornare, lectus sit amet bibendum fringilla, nulla est facilisis tellus, vel faucibus leo elit eu orci. ";
		
	//	Cras risus dolor, elementum quis libero non, vehicula eleifend augue. Proin pellentesque aliquam lorem id condimentum. Proin in erat eu dui tristique bibendum tincidunt ac metus. Donec est tellus, volutpat eget ligula et, porttitor consectetur quam. Etiam convallis justo dui, a porta ante iaculis at. Phasellus lectus nunc, tempor sit amet lectus id, molestie consectetur velit. Nulla cursus lorem ut ante bibendum ultricies. Phasellus id ullamcorper felis. Donec pellentesque eget eros quis lacinia. Phasellus elit ante, aliquam ut interdum sed, posuere vitae orci. Sed at tincidunt mauris. Phasellus mollis velit nulla, vel iaculis eros vestibulum at. Vestibulum tempus libero elementum, posuere velit hendrerit, fringilla nibh. Nunc vitae nunc pellentesque, vestibulum turpis et, vulputate elit. Proin pharetra erat non nisl porttitor molestie. Nulla consectetur ligula et libero accumsan, eget gravida tellus feugiat. Vivamus semper, augue non dignissim aliquet, justo purus porttitor lorem, vitae dignissim lacus metus at lectus. Curabitur quis vehicula nisl. Pellentesque a porta metus. Curabitur vitae erat nisi. Donec eu sem magna. Nulla ultricies magna lacus, in cursus nisl consectetur et. Ut nisl magna, porta ac lacus quis, porttitor malesuada ligula. Duis sit amet varius leo. Nam porttitor congue malesuada. Fusce pulvinar lacus vel gravida laoreet. Proin vitae dapibus tellus.  Aliquam pulvinar velit eget porta dignissim. In fermentum elit et sagittis sollicitudin. Donec vitae eros eget justo consectetur laoreet. Pellentesque varius erat elit, sit amet eleifend diam molestie eu. Morbi vehicula feugiat turpis, ac mattis urna. Vivamus vitae dui tortor. Maecenas tempor, dui et congue consectetur, augue ante pulvinar lacus, vel mattis diam risus in odio. Cras tincidunt, lorem at dapibus condimentum, mauris mauris iaculis velit, at pretium lacus justo non dolor. Nulla nisi eros, varius sed lectus vitae, egestas vestibulum nibh. Sed nec magna quis arcu pharetra posuere. Aenean sit amet ornare dolor, sed pulvinar elit. Etiam eros diam, interdum et urna at, ullamcorper porta massa. Suspendisse sagittis leo lobortis rutrum dapibus. Proin id rutrum ligula, quis egestas risus. Nullam elementum commodo ante ac mollis.  Donec non sapien arcu. Cras fringilla commodo purus in molestie. Duis at rutrum lacus. Pellentesque fermentum accumsan nisi, in dictum est hendrerit quis. Donec consectetur nibh vulputate libero elementum, sit amet tempus felis congue. Integer lacinia nisl nibh, a eleifend sapien congue eu. Aenean quis magna lacus.  Pellentesque nec varius augue. Integer vestibulum mi at justo semper ornare. Vestibulum porttitor nisl non est interdum, ut pharetra ligula consequat. Nulla quis massa at nisl condimentum commodo. Ut ut lacus sapien. Maecenas at dapibus tortor, pretium ornare nunc. Phasellus condimentum justo eget dui aliquet, a rhoncus eros bibendum. Vivamus sollicitudin mauris nec pretium bibendum. Phasellus at leo enim. Suspendisse aliquam accumsan mi eu iaculis. Phasellus non odio ipsum.  Cras dictum bibendum mauris, eu dignissim ante ullamcorper sit amet. Praesent non libero eu sem sagittis pulvinar vel sit amet nisl. Ut iaculis nulla vitae neque consectetur lobortis. Aenean imperdiet ligula in elit tincidunt, sit amet interdum magna.";
	//stparams.stego_data = "159357";
	stparams.stego_size = stparams.stego_data.length();
	stparams.crypto_key = "password1";

	//stparams.stego_data = "asdasd";

	crypto_encrypt(stparams);

	//printf("%s\n",stparams.stego_data.c_str());

    if (!jpge::compress_image_to_jpeg_file("_test_8.jpg", width, height, req_comps, pImage_data, stparams, params))
    {
       printf("Failed writing to output file!\n");
	   getchar();
       return EXIT_FAILURE;
    }
	
	printf("decompress:\n");
	
	stegosystem stparams2;
	stparams2.crypto_key = "password1";
	
	uint8 *pImage_data2 = jpgd::decompress_jpeg_image_from_file("_test_8.jpg", &width, &height, &actual_comps, req_comps, stparams2);
	crypto_decrypt(stparams2);

	printf("%s\n", stparams2.stego_data.c_str());
  
	getchar();
}