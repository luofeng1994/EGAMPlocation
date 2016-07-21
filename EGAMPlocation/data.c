#include "data.h"

float pos_anc[ANCHORS][DIM]={7.90090102756127,-5.40830081762498,37.7536979727496,-9.91553897266907,45.6847614914384,35.3968288342586,3.90695603417256,48.9065976965048,-37.6720559694778,44.2003136939535,-34.5353184255477,-1.38630750703839,-44.8469205847116,-35.4185071332830,-10.5471362340400,-43.3927325732230,40.2056788264087,-44.0558100815624,83.4025744849476,-34.2278175245018,107.529333328898,-5.68843332968486,86.5561282601265,50.3804239384192,96.1987925751440,96.8140559532062,44.0939411179177,89.1857646247001,6.28342457648054,92.4341087150427};
float pos[N][DIM]={12.8673840324943,78.1555006622690,-58.6047031123352,98.6778468071595,-63.1112739368223,-19.4604159602706,-50.4225307419067,-31.8352133243739,-57.8511882642559,-16.0012861103758,-0.253490616888352,-81.9903220246014,-11.0363845394246,-27.2012304223511,-26.1499476768893,-5.11225024707735,80.4021670307379,48.0753839779002,50.8647636126222,89.7658265286108,-23.1450284579238,-46.3900258319959,39.0080084785966,67.1417218163084,-53.3935205502557,43.9205988620913,46.5541815265822,48.1145104045081,92.0987391496742,-9.41919193530940,40.6876444934026,-8.25369349719381,-11.2729950187383,-57.6226368097985,-93.8148942176117,-73.9211110630061,-37.8249422268359,-57.5081060318425,-43.5762796775185,-93.8855711791491,-11.8058239139872,63.9063780622897,-6.67084156073179,-86.1197925635159,-75.1448005129259,90.9696247919030,21.3802814550994,51.8708229424125,8.38661239981640,98.2289939802800,58.3509590053422,-34.3187068625747,-51.4546325967638,-80.9626460034724,-9.33840563570032,-14.4832623223320,28.5223614064512,68.9077419342057,-40.1995579390474,-0.473486745405616};
float pos_init[N][DIM]={17.2810982621078,96.9234054274356,-76.0569546118589,99.7340545300474,-83.8228566640501,-64.4861874722523,-74.6499390382662,-79.9805507690684,-78.6914689329654,-62.0617256459763,-51.0283313693229,-100.922956344991,-16.4711609728419,-59.6970490916764,-48.4953386869561,-58.7590818024667,92.2893746038409,78.7207801696339,61.0001458439446,100,-37.0548447183820,-76.5220653412382,43.9278255771619,89.9083176220692,-77.1258707940097,56.9422349882498,43.2416117093589,63.5062351189496,100,-29.4291750324787,39.6805295880456,-31.9583389535967,-36.4927707851226,-86.8679996355791,-100,-100,-56.8819950429063,-87.9453150237568,-79.8485837149276,-100.490956064151,-13.7973964555241,84.3318810192914,-55.4364214351081,-99.5235044807101,-100,100,25.4546035605904,74.0671365150950,14.8133270106724,99.5832260263632,69.2664772176877,-52.9760956382546,-80.2365177734566,-100.733868797018,-12.5405491551896,-55.3944360215871,35.9499078930648,92.7540508455452,-63.1966225910501,-51.5468163989629};


float rDist[N][N];
float dist_anc[N][ANCHORS];
int net[N][N];
int net_anc[N][ANCHORS];
int net_num[N];
int net_anc_num[N];

float noise_agent[N][N];
float noise_anc[N][ANCHORS];


