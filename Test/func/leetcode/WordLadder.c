#include "test.h"

#include <string.h>
#define string char*
#define int long long
typedef struct _hash_set {
	string* arr;
	int count;
}hash_set;
#define def_hash_set(name) hash_set $_##name = {0}; hash_set *name = &$_##name;

int addr (char * word) {
	int val = 0;
	int i = 0;
	while (word[i])
		val = val * 26 + (word[i++] - 'a');
	// aa = 26, aaa = 26 * 26 + 26

	if (i > 1) {
		int base = 1;
		for (; i > 1; i--) {
			base *= 26;
			val += base;
		}
	}
	return val;
}

void* createDic (hash_set* set, int size, char** list, int count) {
	set->arr = calloc (size, sizeof (string));
	if (set->arr)
		for (int i = 0; i < count; i++) {
			set->arr[addr (list[i])] = list[i];
			set->count++;
		}
	return set->arr;
}
int ladderLength (char* beginWord, char* endWord, char** wordList, int wordListSize) {
	int len = strlen (beginWord);
	int size = 0;
	int tmp = 1;
	for (int i = 0; i < len; i++) {
		tmp *= 26;
		size += tmp;
	}
	def_hash_set (words);
	if (createDic (words, size, wordList, wordListSize)) {
		words->arr[addr (beginWord)] = NULL;
		def_hash_set (source);
		if (createDic (source,size, &beginWord, 1)) {
			def_hash_set (target);
			if (createDic (target, size, &endWord, 1)) {
				def_hash_set (newSource);
				if (createDic (newSource, size, NULL, 0)) {

					for (int _len = 2; source->count > 0; _len++) {
						if (newSource->count > 0) {
							newSource->count = 0;
							memset (newSource->arr, 0, size);
						}
						// itera in source
						for (int i = 0; i < size; i++) {
							if (source->arr[i]) {
								string wd = source->arr[i];
								source->arr[i] = NULL;
								source->count--;

								// each word
								for (int j = 0; j < len; j++) {
									// each character
									char old = wd[j];
									for (char k = 'a'; k <= 'z'; k++) {
										if (k == old) continue;
										wd[j] = k;
										int idx = addr (wd);
										if (target->arr[idx])
											return _len;
										if (words->arr[idx]) {
											newSource->arr[idx] = words->arr[idx];
											newSource->count++;
											words->arr[idx] = NULL;
											words->count--;
											printf ("%s ", wd);
										}
									}
									wd[j] = old;
								}
							}
						}

						// swap source and target
						if (target->count < newSource->count) {
							hash_set *tmp = source;
							source = target;
							target = newSource;
							newSource = tmp;
						}
						else {
							hash_set *tmp = source;
							source = newSource;
							newSource = tmp;
						}
					}


					free ($_newSource.arr);
				}
				free ($_target.arr);
			}
			free ($_source.arr);
		}
		free ($_words.arr);
	}
	return 0;
}

ladder (char* beginWord, char* endWord, char* wordList) {
	int cl = strlen (beginWord);
	int count = (strlen (wordList) + 1) / (cl + 1);
	string* arr = calloc (count, sizeof (string));
	if (arr) {
		for (int i = 0; i < count; i++) {
			arr[i] = wordList + (cl + 1) * i;
			arr[i][cl] = '\0';
		}

		printf("%d\n", ladderLength (beginWord, endWord, arr, count));

		free (arr);
	}
}

test_start (word) {
#define ADDR(str, val) test_func ("%s %d", addr (str) == val, str, addr (str));
	ADDR ("a", 0);		// 0*26^0
	ADDR ("b", 1);		// 1*26^0
	ADDR ("z", 25);		// 25*26^0
	ADDR ("aa", 26);	// (0+1)*26^1 + 0*26^0
	ADDR ("ba", 52);	// (1+1)*26^1 + 0*26^0
	ADDR ("za", 676);	// (25+1)*26^1 + 0*26^0
	ADDR ("zz", 701);	// (25+1)*26^1 + 25*26^0
	ADDR ("aaa", 702);	// (0+1)*26^2 + (0+1)*26^1 + 0*26^0
	ADDR ("zzz", 18277);// (25+1)*26^2 + (25+1)*26^1 + 25*26^0
	ADDR ("aaaa", 18278);// (0+1)*26^3 + (0+1)*26^2 + (0+1)*26^1 + 0*26^0
	ADDR ("zzzz", 475253);// (25+1)*26^3 + (25+1)*26^2 + (25+1)*26^1 + 25*26^0
#undef ADDR

 #define LADDER(s,e,w,count) char _s##count##[] = s, _e##count##[] = e, _w##count##[] = w; ladder (_s##count, _e##count, _w##count)
 	LADDER ("hit", "cog", "hot,dot,dog,lot,log,cog", 0);
 	LADDER ("hot", "dog", "hot,dog", 1);
 	LADDER ("hot", "dog", "hot,cog,dog,tot,hog,hop,pot,dot", 2);
 	LADDER ("a", "c", "a,b,c", 3);
	LADDER ("game", "thee", "frye,heat,tree,thee,game,free,hell,fame,faye", 4);
	// memory overflow
	LADDER ("raining", "cellini", "heaping,conning,nipping,wadding,pulling,lunging,figging,donning,jamming,coating,foaling,ousting,dowsing,busting,penning,lapping,yanking,sapping,tasking,rigging,ranking,larking,farming,dunging,nutting,gouging,barfing,fasting,belting,boiling,boating,dipping,kilning,barking,furling,calving,veiling,decking,ricking,salting,lucking,sending,taiping,marking,martina,warping,bulking,seaming,topping,larding,jilting,besting,weeding,nesting,baiting,jibbing,pelting,bushing,garbing,banting,keeping,venting,rapping,binning,mulling,smiting,hatting,tapping,writing,footing,carding,ratting,bagging,sitting,dousing,pinking,testing,passing,gelling,gassing,ranging,hefting,vamping,wetting,paining,rolling,sinking,yakking,shaking,nabbing,licking,sparing,hamming,celling,halving,matting,landing,kooking,pinning,hagging,narking,soaping,winding,dealing,earring,cunning,moating,skiting,jutting,fueling,hooping,guiling,mapping,hailing,gutting,firming,bunting,mealing,rending,jobbing,pauling,foiling,peeking,rollins,lansing,felling,whiting,vealing,resting,saltine,earning,purging,mullins,pausing,colling,banning,wasting,sealing,gigging,scaring,pocking,massing,curring,storing,dinging,handing,pitting,faining,cupping,staring,riffing,gowning,hipping,vanning,darting,maiming,damping,deaning,bellini,kipling,marting,hawking,fending,kicking,beading,curving,wending,yelling,foaming,rifting,surging,gaining,stoking,panging,winking,nursing,oinking,looking,tolling,bailing,tanking,hacking,warming,cooping,wanting,rotting,kinking,bugging,purling,wincing,joining,belling,wilting,tensing,fellini,wilding,binding,bugling,sagging,nagging,binging,tatting,cellini,silting,belying,ripping,crating,slaking,killing,hurting,running,harming,banding,rinking,staying,touting,hasting,melting,nibbing,talking,ganging,bonging,rilling,damning,pooling,porting,sinning,collins,barbing,bunking,smiling,hanging,tending,bulging,ginning,coiling,lolling,molting,letting,mending,hinging,gunning,melding,dilling,shaving,harping,basting,cobbing,carting,leading,styling,fowling,goading,yowling,zipping,wagging,gaoling,panning,valving,peeling,titling,sailing,harding,parring,haloing,quiting,punting,reeling,batting,signing,pegging,bogging,mashing,rankine,seeding,sassing,wafting,winging,framing,rooting,longing,sabling,bulbing,whiling,balking,canting,dashing,dueling,renting,booting,whaling,vatting,veining,fencing,yucking,slaving,welling,sunning,lulling,purring,dawning,sensing,meaning,wording,hogging,parsing,falling,yelping,dinning,vetting,hulling,reading,lapsing,tooling,hoaxing,roiling,forming,ramming,gelding,felting,popping,walling,costing,welding,washing,filling,lasting,couping,cabling,getting,winning,carping,martins,bilking,burning,jelling,sicking,tinting,ceiling,totting,balding,kenning,tinging,hugging,westing,burring,pasting,pecking,parking,slaying,pigging,heating,manning,bucking,bussing,gagging,goaling,rowling,netting,funking,pitying,jarring,tasting,putting,beating,funding,mauling,balling,molding,shining,perkins,dialing,panting,looping,welting,relying,dulling,dumping,tanning,warring,gatling,staging,finding,farting,petting,picking,swaying,toiling,jambing,bawling,minting,wedding,hulking,keeling,nanking,railing,heading,cutting,gosling,vesting,sighing,mucking,copping,polling,raising,fooling,hooting,titting,calming,seating,rifling,soiling,dubbing,jesting,posting,sacking,corking,yipping,lathing,bopping,setting,coaxing,poshing,fawning,heeling,warning,napping,vending,mooting,hurling,supping,nanjing,pipping,tagging,mopping,souping,palming,gulling,kirking,gilding,docking,wefting,dusting,sharing,darling,bowling,lauding,bidding,hopping,honking,hunting,pepping,busying,damming,patting,hitting,gusting,jigging,gabbing,hosting,sidling,telling,rusting,daubing,reining,memling,healing,gashing,betting,lilting,hashing,salving,firring,gabling,ducking,waiving,skating,worming,waiting,burying,booking,corning,suiting,hooking,gonging,listing,hulaing,sulking,digging,fouling,zincing,cocking,packing,scaling,pooping,zinging,banging,bolling,punning,palling,sipping,bunging,minding,choking,yapping,nicking,warding,gorging,canning,culling,lending,spaying,lashing,pupping,fanning,banking,pinging,roaming,sopping,fonding,searing,fucking,rooking,tooting,raining,billing,pulsing,curbing,cashing,calking,harking,tarring,tacking,whining,tarting,pauline,rasping,howling,helling,curling,pucking,hauling,coaling,lopping,mailing,wailing,lugging,ticking,staving,snaking,selling,masking,jabbing,mewling,heaving,soaring,fagging,cording,begging,ridging,jetting,backing,dotting,lacking,parting,jotting,dunning,tinning,stiling,stating,zapping,hearing,fitting,barging,galling,wigging,feeding,tenting,looting,cabbing,cursing,dunking,dabbing,ragging,bedding,witting,pouting,burping,slating,tamping,basking,failing,papping,narcing,lancing,furlong,tabling,dolling,tailing,pawning,collies,lamming,coifing,bolting,sucking,rafting,morning,ranting,tabbing,rinding,bandung,bashing,bending,ducting,casting,camping,flaming,hinting,sanding,carving,lagging,helping,keening,jolting,temping,junking,manging,dimming,ringing,tipping,spiking,malling,pursing,soaking,willing,fulling,causing,jacking,furring,singing,halting,tucking,ruining,denting,calling,barring,fopping,yawning,tilling,nilling,downing,cooling,martini,budging,lapwing,mincing,rinsing,cowling,marring,coining,sibling,potting,tauting,bulling,lurking,sorting,poohing,bathing,spicing,nailing,spiting,racking,lusting,rutting,gulping,tilting,pairing,peaking,capping,gobbing,finking", 
		5);
#undef LADDER
}
test_end ()