#ifndef ATOM_H
#define ATOM_H

extern int Atom_length(const char* str);
extern const char* Atom_new(const char* str, int len);
extern const char* Atom_string(const char* str);
extern const char* Atom_int(long n);
extern void Atom_delete(const char* str);
extern void* Atom_find(const char* str, const int len, unsigned long* hash);
static unsigned long scatter[] = {
	915791453, 122126246, 467127611, 800412297, 229564755, 227517620, 942291974,
	354027291, 598668846, 941153337, 557203521, 722622874, 681538352, 836608704,
	681294485, 498812588, 197733260, 818150635, 631756971, 734041123, 807989998,
	229826708, 40693123, 735607342, 169759031, 788060304, 864776718, 480575751,
	717323885, 123605206, 650286498, 495133519, 130358835, 597048964, 862843629,
	690775930, 73252951, 341139673, 61321362, 383902413, 862778826, 934146114,
	221506166, 825079909, 416872288, 858107258, 137604165, 611199388, 114089073,
	121639265, 455494113, 249752583, 623249662, 761639695, 962849338, 929324696,
	839529013, 503171034, 426250933, 105663672, 409791822, 569150734, 777348240,
	435025938, 232992593, 688212704, 683153932, 176965432, 542012234, 235144206,
	760172474, 948228608, 107321467, 148834871, 281407039, 40521441, 528078281,
	842616057, 127038152, 810683909, 44460186, 510722910, 289028369, 927717400,
	90163877, 545903940, 91219068, 7912873, 973008571, 157616759, 130322588,
	628295737, 261526897, 905609330, 173423876, 223797302, 386294514, 39686074,
	558082977, 62701940, 904007398, 986695444, 929250452, 722983276, 378159260,
	211701050, 405677246, 408079319, 606752328, 872788039, 107585319, 7005941,
	589356131, 852940158, 457535868, 472524333, 459516517, 508090535, 395304352,
	334694080, 292904683, 396173206, 961097323, 362892816, 341409366, 13499651,
	1406925, 975864663, 146431138, 704080984, 846189578, 307904536, 232422218,
	195483220, 761686486, 443215705, 439014370, 124371830, 576984287, 742112867,
	502448759, 744269403, 503818075, 599360277, 976600201, 72049416, 431891141,
	183110496, 970688835, 842036488, 845385203, 592900798, 906014851, 918791682,
	519199743, 214114465, 600977473, 86863978, 817850643, 630216240, 547729974,
	182226766, 363294114, 330379513, 143622182, 908077963, 465308828, 921830108,
	857873026, 279549296, 813389833, 603005884, 892926438, 924996359, 259092241,
	894057563, 161356389, 392533569, 934866179, 602782793, 142566, 771314512,
	819780701, 747753527, 614413631, 470692303, 911281890, 700474816, 210555623,
	639812257, 609867531, 560278934, 142410954, 886751661, 223956336, 505852859,
	85438093, 366910079, 719496242, 170382050, 968243238, 125038845, 256746317,
	270684613, 404126304, 282186300, 849355826, 79059788, 643217570, 192286099,
	473690334, 294637319, 993576135, 974361830, 625727070, 994091368, 72943152,
	595079838, 306858311, 66290100, 637724413, 903446727, 458811033, 367820523,
	544543587, 637826036, 545272044, 272084098, 419918506, 212718778, 607966729,
	171165189, 250090364, 903177664, 428536932, 180164065, 562447287, 454076613,
	504850407, 999255605, 996701369, 747841932, 989461266, 344182979, 157862667,
	905472127, 916107445, 355912615, 189113001, 11107047, 794050181, 933984217,
	762737979, 937479282, 524993598, 34220324,};

#endif