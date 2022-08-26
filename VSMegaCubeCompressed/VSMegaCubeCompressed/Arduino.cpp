#include "uncompress.h"
#ifndef _WIN32
#include <avr/pgmspace.h> // allows use of PROGMEM to store patterns in flash
#else
#include "arduino_util.h"
#include <iostream>
#endif

const unsigned long PROGMEM animation1[] = { 1420UL,3UL,2532875759UL,3083136890UL,3190258644UL,4047232677UL,2313090349UL,1324848495UL,2008853372UL,3185794018UL,4011515667UL,2018965661UL,3266823407UL,3854444411UL,703677407UL,1334451957UL,2085681065UL,3800546634UL,339602003UL,2716805791UL,259609848UL,2076616645UL,3728031272UL,4037669187UL,2236582430UL,3397144823UL,870500287UL,2669034986UL,4172410705UL,3314514572UL,746312807UL,1675514686UL,519215601UL,4153200523UL,3160833112UL,3778273991UL,161420861UL,2365108719UL,2009427839UL,3190520788UL,4049329827UL,2329867549UL,1459071215UL,3082594172UL,3185916898UL,4011450133UL,2026829997UL,3262629231UL,331230075UL,502356958UL,2407288063UL,2078435241UL,3742580044UL,4170836579UL,3301921566UL,645486839UL,868927422UL,2654354929UL,4054970249UL,2240773196UL,746316391UL,1675563836UL,519612926UL,4156903250UL,3190454928UL,4048802951UL,2325652541UL,1425187311UL,2811563900UL,1013480418UL,3812876053UL,169801897UL,1358415183UL,2277386872UL,3186711548UL,4018855587UL,2086073629UL,3803687151UL,364726139UL,2917481439UL,1865015032UL,2026829765UL,3329736235UL,331215195UL,2649721566UL,4017903344UL,2078439416UL,3742612811UL,4171098715UL,3304018654UL,662345463UL,1003141054UL,3730161137UL,4054708105UL,2372893774UL,729539191UL,1541346236UL,3740835297UL,1203630066UL,2448391106UL,1870389872UL,1809781637UL,1425507729UL,4152725844UL,2381390932UL,4050903792UL,2078470285UL,3740318243UL,67241987UL,544988948UL,3186704448UL,4010352402UL,182614201UL,1190641187UL,4156842332UL,3156573266UL,2945113872UL,2078414724UL,3800080936UL,134483835UL,3740833320UL,4156932387UL,3185652824UL,3726123207UL,406201467UL,1167031372UL,4045685271UL,2286077885UL,1090895343UL,8515448UL,4045128200UL,2017786033UL,3742744099UL,3083104008UL,2380340194UL,578544328UL,1273108412UL,3742500081UL,1084106363UL,3052501986UL,2945015536UL,601814917UL,3740779745UL,3619563274UL,2851015458UL,4010484392UL,467814569UL,3806840289UL,4156940571UL,3185669190UL,134483975UL,1089977896UL,2078441600UL,3725737509UL,365228403UL,2381282374UL,4018717368UL,2018179237UL,1595260449UL,4156829449UL,3305194576UL,268967671UL,3186699344UL,4018897479UL,2076338353UL,3157279119UL,812402935UL,2334062744UL,3796403246UL,277188475UL,2181790687UL,17030896UL,3795289104UL,4035572067UL,3190520902UL,1871240721UL,465713093UL,1157088657UL,2546216824UL,3190032866UL,2168212727UL,1810036676UL,1595063777UL,1203629835UL,3186592194UL,2944159253UL,1407063621UL,3726001489UL,935629139UL,3318713282UL,4018913847UL,2076371085UL,268967951UL,2179955792UL,4156883200UL,3156507722UL,730456807UL,467597452UL,3742467441UL,4036358475UL,3190520898UL,4018691602UL,2315421857UL,537935343UL,2078431392UL,3742827663UL,4152676707UL,2019590942UL,1624805871UL,373158192UL,3297839197UL,554376951UL,68614078UL,34061793UL,3295610912UL,3776176839UL,2086074509UL,3742481443UL,931426186UL,2314177314UL,797466352UL,2085098437UL,41458159UL,3620073353UL,3190127554UL,2407259670UL,2078217092UL,1593351211UL,2814127243UL,3157035682UL,1871258279UL,2342459268UL,3742860399UL,4152742171UL,537935902UL,64944288UL,4018799105UL,2018048149UL,1460913615UL,935194904UL,3189967586UL,3777749655UL,2086074501UL,3742415909UL,335876419UL,1075870687UL,4156862784UL,3190688030UL,4010386119UL,4039181885UL,3249611742UL,746316384UL,2300711098UL,1108753903UL,137228156UL,68123586UL,2296254528UL,3257386383UL,4172149019UL,3189995590UL,1862852373UL,333387332UL,1594932705UL,4170196874UL,82916318UL,2945179410UL,2085287813UL,519552045UL,4156434185UL,3186702422UL,1333287190UL,2019104069UL,3742516559UL,389951240UL,3190753503UL,4010517047UL,1075871805UL,129888576UL,3742630914UL,4036096299UL,2921827230UL,1870389808UL,2084967876UL,3260532015UL,4172149003UL,3189864522UL,671752839UL,2151741374UL,4018758272UL,2086408765UL,3725804943UL,3783396475UL,2204256189UL,1492632769UL,306454900UL,2217507807UL,274456312UL,136247172UL,297541760UL,2219805471UL,4049330743UL,2085023885UL,3725704747UL,666774664UL,3189865410UL,4045426452UL,165832637UL,1595391524UL,4170575627UL,1039104090UL,4017901074UL,2078437549UL,2666574381UL,4038208138UL,3190065822UL,779902481UL,2086539710UL,3726066799UL,2151743611UL,259777152UL,3190294532UL,3777225303UL,1548687165UL,3740779617UL,4169935752UL,2226096734UL,4049330711UL,2084761749UL,1343505679UL,8515452UL,3742549249UL,4172817531UL,3156642590UL,3271825655UL,113545083UL,2985265539UL,612909800UL,140048318UL,548912625UL,272494344UL,595083520UL,144643646UL,3803694191UL,4170047771UL,3156442198UL,1333549329UL,2084763524UL,3795885609UL,331665275UL,3190783048UL,4046183958UL,2078208181UL,3740834852UL,4156875099UL,1038181466UL,3781448981UL,2085164349UL,1559804963UL,4173079420UL,3157166302UL,8519927UL,519554305UL,2085621768UL,3259483311UL,3097374331UL,3186591938UL,4044904209UL,157226173UL,3803694127UL,4169523499UL,2687011358UL,17030904UL,3190131202UL,4050667767UL,2018317885UL,2248684015UL,227090167UL,1675563782UL,1225819601UL,280096636UL,1097825250UL,544988688UL,1190167040UL,289287292UL,3312421086UL,4045128247UL,2017917101UL,2667098659UL,4169527048UL,3296803922UL,663330551UL,2086598800UL,3797400621UL,4156416363UL,3186702408UL,4018782903UL,2076362933UL,3267930666UL,4170328699UL,3119609926UL,4051191544UL,2019365309UL,17039855UL,1039108610UL,4171243536UL,2223999326UL,1899781367UL,2078216581UL,3794841123UL,314452347UL,3312420958UL,4044079703UL,1079055421UL,34061809UL,2085295108UL,3806368239UL,4036635771UL,202400734UL,454180335UL,3351127564UL,2451639202UL,560193272UL,2195650500UL,1089977376UL,2380334080UL,578574584UL,2329874876UL,3795289199UL,4035834203UL,1039230022UL,4044086801UL,2298640549UL,1326661103UL,4173197600UL,3299833946UL,1379139287UL,2089100616UL,3740780518UL,4156434971UL,3186708702UL,4018833143UL,2085894077UL,3802250735UL,353234811UL,2825878495UL,1132191480UL,467597253UL,3740779572UL,4156838931UL,3295937502UL,2225331959UL,1159133118UL,678936052UL,1102966691UL,2827975647UL,1132191519UL,467597565UL,3740780517UL,4156440360UL,3186751686UL,4019177015UL,2088636861UL,3270544879UL,4111069071UL,166694850UL,4019176181UL,2411917961UL,3270674927UL,4052348815UL,2314178498UL,4019176177UL,2411917449UL,2666564079UL,4110545400UL,32635038UL,4059602677UL,4171166337UL,2666693105UL,4051825144UL,2180118686UL,4059602673UL,4171165825UL,3740303857UL,4155121435UL,3176200414UL,3850880759UL,742275005UL,3790716399UL,260960120UL,2087680967UL,3816545816UL,467595459UL,3304565279UL,396889999UL,2823781502UL,1132191519UL,1360983997UL,3578789887UL,4115922943UL,4250206207UL,4281155583UL,4291514367UL,4294038527UL,4294735103UL,4294905151UL,1379124047UL,2089100616UL,2233679174UL,1741181524UL,1213346772UL,1182565669UL,1418011453UL,3563571282UL,625496627UL,1017543813UL,1381270819UL,868902856UL,2233813074UL,591160956UL,3360838789UL,1379124839UL,2089100616UL,2233678886UL,1741181524UL,1213346754UL,1182565669UL,2552496705UL,37853185UL,1208255140UL,2200961840UL,151341060UL,1611030672UL,177211654UL,213983250UL,538053824UL,2185241621UL,605650969UL,2152213056UL,855651076UL,2421456968UL,6686976UL,2835386470UL,3423732000UL,18926592UL,604127570UL,1100480920UL,75670530UL,805515336UL,88605827UL,106991625UL,2416510560UL,3240104458UL,302825484UL,1076106528UL,427825538UL,1210728484UL,3343488UL,1417693235UL,1711866000UL,9463296UL,302063785UL,550240460UL,37794305UL,2550241316UL,2170815041UL,53495812UL,3355738928UL,1620052225UL,151412742UL,538051472UL,213912769UL,604184594UL,2149155392UL,4265616409UL,579478056UL,541591486UL,2149513213UL,4156548144UL,3219162128UL,4028895991UL,4237261791UL,4280540919UL,2055796669UL,2669134927UL,4173209978UL,3320926238UL,4015038199UL,1386412189UL,3742876879UL,4173210962UL,2985251614UL,1867522296UL,1369380805UL,3740843562UL,4172934993UL,4050375712UL,2333362117UL,3562520111UL,3367904635UL,3320914900UL,797496535UL,2077550526UL,3564682794UL,2277896523UL,3190786580UL,796644503UL,4155049924UL,676301794UL,1870421783UL,2085681064UL,3799492913UL,321474443UL,2842655839UL,3514582775UL,2346854313UL,1594927599UL,4110544764UL,439323358UL,4051457269UL,2346882333UL,3740606959UL,4114739475UL,1043302494UL,4051459744UL,870228285UL,1587922417UL,2789093257UL,3186719816UL,4050378406UL,3314503745UL,726753047UL,1369372862UL,553172463UL,3807951338UL,398830107UL,1038754783UL,3932970770UL,1944254605UL,3742876746UL,347990651UL,2716484703UL,3930841848UL,1407129540UL,2297937386UL,2721967994UL,233798626UL,4017873429UL,2078418185UL,3795124719UL,329903995UL,2907405279UL,3929792248UL,1356789701UL,2297937454UL,1472065898UL,1039108884UL,4017901731UL,2078426409UL,3740772815UL,4170836515UL,2649707294UL,4017788664UL,1406445729UL,3735305713UL,4112380251UL,2832294878UL,1266410103UL,1407354813UL,1654111722UL,4156911442UL,3314514574UL,2944128103UL,2078187585UL,3742541967UL,322784635UL,3186254815UL,4015185683UL,848761789UL,3569860081UL,2780688699UL,703677406UL,290709237UL,2078217385UL,3742711113UL,364767811UL,3186577887UL,4048719637UL,1540655037UL,3735272945UL,4112118115UL,3557848030UL,4111593339UL,3926948830UL,1394763709UL,2819079663UL,1042512862UL,2972442357UL,64944041UL,306048850UL,1039104669UL,2078209296UL,519552324UL,1203629905UL,300907476UL,4045135605UL,2300508073UL,1257749838UL,1740499564UL,1039094562UL,4017787157UL,3186231353UL,2077556983UL,4155048431UL,4015064030UL,3735095229UL,4112395131UL,2842770398UL,1250565879UL,878245821UL,551009770UL,347991888UL,1039104659UL,2407259810UL,2085156776UL,3742547278UL,4172409451UL,3186577822UL,4017786645UL,3742711869UL,4155010939UL,2705951326UL,3931365112UL,3558930365UL,2713579403UL,975469534UL,1870417653UL,2204039081UL,3794255168UL,4156375163UL,3186236386UL,575856403UL,1344206782UL,2331493807UL,4169789306UL,2582608512UL,712758472UL,2085266494UL,3805623791UL,2812850043UL,1028160482UL,1081859857UL,1138686121UL,3740843498UL,331214672UL,3314514583UL,4017869927UL,3796347137UL,2545766011UL,3314174946UL,4015103735UL,2056983229UL,3563631087UL,666771835UL,1042709460UL,595320486UL,2338256190UL,3742410927UL,329797499UL,3108731871UL,3928219384UL,1407121549UL,2398662127UL,2829184890UL,3291446228UL,595317799UL,2317264830UL,1559570927UL,346591100UL,3175709663UL,1115414288UL,1407354813UL,3740839402UL,129888081UL,4046184098UL,2078209333UL,485997892UL,2078426372UL,3742476751UL,347950971UL,3186248671UL,642917031UL,2056845246UL,3561473487UL,2789077299UL,502234198UL,1148968693UL,1360983997UL,3561471471UL,4195479419UL,3164896222UL,2944966392UL,64944068UL,1138693654UL,3742471569UL,3164736379UL,260824848UL,2309150612UL,2197263791UL,1200885628UL,2448509922UL,2944131618UL,4018686529UL,387254205UL,2513511391UL,2972966678UL,2078216853UL,3802300935UL,2997496955UL,2922019810UL,662457074UL,2085376702UL,3395851759UL,2546323787UL,1039108554UL,746143808UL,2035873954UL,521190952UL,4156434474UL,3796009024UL,675360635UL,1659354665UL,289795882UL,3293522911UL,3843285719UL,1541580973UL,1190698469UL,389851004UL,1018068041UL,2860275989UL,2329874876UL,3798439023UL,4288278827UL,2851150814UL,545015031UL,2287902721UL,1207968783UL,545018882UL,2148679681UL,37789988UL,1224746112UL,21270530UL,76677193UL,2449475872UL,1074340866UL,2359314UL,153122112UL,1610617344UL,1364885504UL,65279940UL,3841261577UL,2449475843UL,1209996544UL,537169920UL,9447497UL,2453670176UL,1079059456UL,19169298UL,2759852616UL,2416068864UL,589828UL,38280528UL,134218880UL,341221376UL,2667087345UL,4156436304UL,3186719366UL,579523639UL,333385918UL,1196266UL,4194340UL,306709760UL,1207968896UL,1208647680UL,2396162UL,344981577UL,76546120UL,2449492256UL,37812392UL,1224746112UL,1090075970UL,1076133890UL,199370925UL,2147779058UL,299066UL,3145737UL,76677440UL,301992224UL,2449645568UL,1074340864UL,86245394UL,19136530UL,612373064UL,9453098UL,2453670176UL,130297680UL,19136530UL,521223658UL,4154982472UL,3176106206UL,3932441847UL,2216497853UL,3901876736UL,1108345888UL,138027856UL,1108378625UL,34608392UL,277094664UL,271124546UL,2216757314UL,1359479312UL,554189328UL,135172UL,138543265UL,33555488UL,1218527232UL,368020386UL,3481002693UL,2078216616UL,3796404559UL,87682954UL,8652866UL,8UL,2231910922UL,268503056UL,138417152UL,554307717UL,69730832UL,557977633UL,2315797124UL,270599621UL,3934527810UL,69253301UL,3887941698UL,4111500002UL,3926948830UL,1356789701UL,1123584555UL,61362044UL,9437257UL,335544576UL,2449492256UL,73728UL,1074339882UL,19136658UL,18895424UL,612373056UL,3802286081UL,2449475843UL,4111016960UL,590820UL,2729771154UL,130559880UL,1089077266UL,2359314UL,83886272UL,612373064UL,2147502080UL,2416068618UL,4784164UL,4723856UL,1226835088UL,4171796992UL,612368960UL,3175237888UL,4726544UL,4154983568UL,18906180UL,595320480UL,870249790UL,1257744785UL,2818707572UL,277062215UL,2369849408UL,268706024UL,1411450946UL,2216691776UL,554189328UL,554188964UL,138547332UL,138495008UL,1074824225UL,42205192UL,276832520UL,2048UL,780739205UL,2333366206UL,1185103343UL,624815996UL,3190524194UL,554177767UL,2097680UL,136839168UL,541201441UL,1342193668UL,1108615424UL,138445864UL,1082396832UL,168854792UL,387328033UL,2231910532UL,3805784592UL,554176642UL,193699573UL,4017869890UL,3740793921UL,4156379016UL,3186259016UL,4015219287UL,2056852285UL,16908783UL,537885652UL,2078187648UL,3735564837UL,278651UL,260595445UL,2086577221UL,777011631UL,1426126782UL,4170051467UL,2381406848UL,676372216UL,467597466UL,3742680202UL,373638267UL,2235923423UL,3481002664UL,2085294376UL,1648699887UL,2789076872UL,1039108706UL,696009380UL,2291431358UL,2365105647UL,1740501298UL,3190444064UL,3175104759UL,3929822967UL,1138940077UL,1157146090UL,338169980UL,2155903967UL,129888586UL,3481965570UL,2329184197UL,345488940UL,4156845962UL,3190323852UL,680678567UL,2329208766UL,3573005862UL,4156895507UL,2585885324UL,2404556536UL,64951876UL,1559793130UL,4169788028UL,2246706142UL,3927170808UL,64950589UL,3371679210UL,356839295UL,2653845599UL,4048049912UL,153024657UL,521518633UL,3902402315UL,3190098722UL,621486839UL,877459390UL,1492633025UL,4156932404UL,1038171222UL,663330578UL,2330085308UL,3742532687UL,1351217491UL,1154448354UL,2407291923UL,2086409029UL,1108788399UL,3633180540UL,3297740866UL,645421303UL,2216720318UL,276030183UL,2918282178UL,4018733688UL,542053181UL,1595325937UL,306556792UL,2985265247UL,4018686712UL,621414333UL,337049534UL,3186705482UL,4018897479UL,2076325829UL,1190694443UL,323317624UL,2381281375UL,4018692328UL,4044097469UL,182609853UL,3802644011UL,4156435210UL,2084966656UL,814231023UL,3186717666UL,4048834288UL,2085548221UL,3725704743UL,331698555UL,2448503774UL,4045652600UL,2019096709UL,3740834340UL,4171902729UL,3190753758UL,4010402471UL,1011939465UL,2668707297UL,314318978UL,2215656415UL,192668247UL,4156435722UL,3742795008UL,277172555UL,2854648799UL,1870389776UL,2078150721UL,4035051584UL,1043102792UL,4049882128UL,2085361469UL,3255310831UL,406957947UL,1039209438UL,730422808UL,734136254UL,1157088593UL,4170838344UL,3190360222UL,554345175UL,1414330302UL,3740779553UL,4170719362UL,68170718UL,175890195UL,4156435722UL,3742795008UL,344314155UL,2245086175UL,8118036UL,519552034UL,3314513996UL,645416967UL,161419966UL,3803694287UL,2560749323UL,2716797922UL,3775128200UL,2085419177UL,2668968992UL,322710666UL,1009552287UL,4018741011UL,2334070404UL,3742794991UL,310751539UL,2242999263UL,125558546UL,2313089154UL,2651865103UL,364783499UL,3190062162UL,1871331911UL,2076362693UL,1120451110UL,3367890825UL,1039108418UL,4050668819UL,1884100157UL,1326824945UL,1740935032UL,2448361954UL,2702435000UL,2085943236UL,3740832802UL,386191667UL,80831455UL,2972442391UL,2078216325UL,3792746019UL,373158179UL,502237598UL,4172931336UL,1559266332UL,1084107644UL,3301932002UL,2944157715UL,1809717189UL,3793208465UL,314453115UL,2179955682UL,3186592066UL,1360168197UL,3767660412UL,2381413346UL,596303600UL,1541280190UL,3726099665UL,406463355UL,32472034UL,2582608963UL,4018848440UL,2328017065UL,2218802671UL,192667409UL,1559792674UL,331100028UL,3157228629UL,1871306515UL,1415509892UL,1291289841UL,4036895868UL,1042381896UL,646520342UL,2344777662UL,3262627370UL,2024402827UL,1042447298UL,1333284628UL,136357828UL,251118767UL,4170572036UL,2336391902UL,2705341935UL,1414723525UL,2667037729UL,4169411209UL,2215664606UL,2148540151UL,521288736UL,319033227UL,97483865UL,108781331UL,1809772674UL,1157072400UL,1468403580UL,2179959522UL,4048833264UL,2085753149UL,1109900838UL,2830495626UL,1039108162UL,4043855123UL,136361917UL,217562662UL,4156866820UL,2336391838UL,2705341935UL,1951070149UL,1391969569UL,4156891508UL,4010311710UL,3725789117UL,302117219UL,3299838366UL,4046701159UL,2076375213UL,3271075368UL,4171376763UL,1042970688UL,1325932307UL,186820549UL,585614543UL,1204080395UL,3315678178UL,2705056455UL,1816983493UL,1559741601UL,4156858732UL,3156247006UL,268690039UL,2313348996UL,521438575UL,2425483145UL,2985384930UL,780771056UL,2078429362UL,3796939311UL,323276667UL,1167031388UL,1870421011UL,601756613UL,3262683633UL,4172162171UL,1039214660UL,621322005UL,877459390UL,1492633025UL,4156932404UL,3290524746UL,763944695UL,2085156996UL,3798435215UL,2017593211UL,554122798UL,2329876284UL,3740831783UL,302313779UL,2247191519UL,4045691639UL,2334062725UL,3740828847UL,3796934779UL,2705420155UL,2078217284UL,1593358372UL,3082560380UL,2221902050UL,4046739191UL,2296313733UL,3740829198UL,3742417019UL,2964203131UL,2922019810UL,259777200UL,4156842256UL,587760350UL,199169854UL,3742713071UL,2009394955UL,3185646562UL,3778827543UL,2300515133UL,1595030567UL,1199935368UL,2649721250UL,4018840304UL,2078470301UL,2666569258UL,331681672UL,703665118UL,4046176776UL,2017526717UL,2696396325UL,259777232UL,3186714704UL,4043855124UL,178415549UL,385334817UL,129884420UL,2399674372UL,65193924UL,3742778561UL,4156866931UL,3156246942UL,3783531255UL,2212434237UL,3803747809UL,314318971UL,2219801685UL,4018773780UL,2078445705UL,3792742957UL,356831099UL,770669634UL,4018766344UL,809567037UL,2695347697UL,259777232UL,3186714704UL,4048051474UL,1214059709UL,537378289UL,3253697474UL,2086205316UL,3798434944UL,4036620587UL,3190520908UL,696917744UL,2317283518UL,2651864616UL,130338698UL,2851165122UL,4046701256UL,868403385UL,1324913137UL,4172411240UL,1039231054UL,4045953811UL,677058365UL,544718321UL,4171099912UL,3118531584UL,559669880UL,2078427077UL,1459135524UL,1740385404UL,2485947298UL,747364088UL,870250644UL,3742827729UL,386191715UL,2230410207UL,645679351UL,2300506270UL,1242644011UL,667242364UL,1039108418UL,4050668819UL,1884100157UL,1527954929UL,935194888UL,1042352994UL,3781418770UL,2178873497UL,3482033680UL,3318679617UL,604627639UL,1011677118UL,1526187137UL,381542780UL,3189865545UL,556524231UL,673118142UL,1541339284UL,3306235984UL,772349687UL,148838334UL,3795365359UL,364765467UL,3185671245UL,562274871UL,807335870UL,2308896708UL,2667097131UL,4172032906UL,407110622UL,548912625UL,3318712514UL,1362871031UL,1002633157UL,1190698481UL,356904824UL,3301931087UL,2399789639UL,1675761604UL,3798503905UL,2830903163UL,3156704226UL,663396088UL,1138894780UL,1157072113UL,4035054204UL,1089436746UL,4018757136UL,4172556333UL,1511261150UL,356855676UL,2582608962UL,4018684584UL,2017527229UL,134283759UL,2985370562UL,2435858680UL,811399109UL,543669745UL,113110792UL,3293542497UL,628646135UL,136247470UL,3742713007UL,1740972307UL,2753723362UL,713776888UL,870250628UL,3742401873UL,1619135355UL,1087339490UL,4017900560UL,4172556329UL,1511261150UL,390377340UL,1160739922UL,4017902103UL,3740305409UL,1038168187UL,3776978710UL,2085288089UL,3742579750UL,2277257098UL,3157232226UL,260711047UL,870492036UL,1425048049UL,4038208124UL,2989390924UL,612125936UL,2086278078UL,1511243308UL,381996924UL,3318709322UL,4017900054UL,2076313629UL,4030730255UL,2384693475UL,1334402808UL,736032949UL,1593296170UL,4156433746UL,3186690714UL,4044522743UL,847312829UL,2496118257UL,2755522442UL,300907612UL,1870390005UL,2078217640UL,3740835143UL,4156844635UL,286282718UL,69905UL,1084227584UL,644244548UL,1073742370UL,2576421121UL,1145883865UL,2575334020UL,1718406361UL,805813574UL,697932595UL,419424UL,1717567488UL,3328598102UL,3276UL,1717608448UL,2576141510UL,1094713369UL,214746180UL,2433816984UL,3257016442UL,1145325158UL,2219313085UL,3618341959UL,310952488UL,79833285UL,3256125987UL,2465993476UL,4158691362UL,703578772UL,41669772UL,198462515UL,361128344UL,3425897633UL,1637106344UL,2354061126UL,1352596018UL,286331152UL,1UL,1682194592UL,35792486UL,285294592UL,3436812689UL,2055488588UL,3436812672UL,3175507564UL,2603823111UL,1717578137UL,6UL,1616275040UL,214746726UL,2684354560UL,3435554400UL,1677708UL,3292807488UL,429395148UL,544903441UL,1181139490UL,129844292UL,2219279432UL,3257456555UL,684003976UL,2317550786UL,319078932UL,2225246972UL,1386150930UL,142608512UL,142608512UL,2165866656UL,2165866776UL,411123992UL,413341859UL,2460162211UL,684339852UL,3435924115UL,3435973836UL,806669516UL,817901760UL,683684032UL,2353040448UL,2353040448UL,2351105104UL,1095025200UL,1644454404UL,2718196228UL,3392310418UL,3423245096UL,3435973836UL,348966092UL,3224420400UL,3224420400UL,1082933288UL,1082933388UL,587485324UL,1153577100UL,73532481UL,73532514UL,848073378UL,177416394UL,3435973836UL,3435973836UL,817901588UL,817901760UL,2353014976UL,2353040448UL,72387648UL,3257961507UL,1644446020UL,1644454404UL,2358419972UL,2468923954UL,3435973642UL,3435973836UL,3224376524UL,3224420400UL,1076412464UL,1082933388UL,1351368844UL,814490372UL,71386306UL,73532514UL,2460091490UL,684339852UL,3435924115UL,3435973836UL,806669516UL,817901760UL,683684032UL,2353040448UL,2353040448UL,2351105104UL,1095025200UL,1644454404UL,2718196228UL,3392310418UL,3423245096UL,3435973836UL,348966092UL,3224420400UL,3224420400UL,1082933288UL,1082933388UL,587485324UL,1153577100UL,73532481UL,73532514UL,2251391650UL,2251392561UL,3322218033UL,3321939456UL,3389048320UL,344269128UL,2569488850UL,1714631426UL,102UL,858981632UL,3435947622UL,571080704UL,270672418UL,3574665489UL,130809611UL,572661780UL,1145325090UL,1717577796UL,6UL,1615960472UL,1717986918UL,2791728742UL,572802112UL,2291208450UL,3220225352UL,570507388UL,1147281954UL,42222660UL,26214UL,429490176UL,1717986821UL,1717986918UL,1145309798UL,286269986UL,198477961UL,336055295UL,572662304UL,1145324614UL,107372584UL,2550136832UL,1717588377UL,1717986918UL,1084647014UL,35791940UL,1216909585UL,2092953789UL,572653888UL,1145332258UL,1711440964UL,102UL,85563776UL,1717986918UL,1717986918UL,574899210UL,2299596834UL,4278965384UL,538183627UL,1176642082UL,675562564UL,419424UL,2576875520UL,1717985361UL,1717986918UL,1145087590UL,285352484UL,3175647377UL,1081917424UL,572662273UL,1145324642UL,1717961348UL,2147483648UL,1711610265UL,1717986918UL,174483046UL,572671044UL,2290684176UL,3422489556UL,572527623UL,1145446946UL,4154999876UL,1181114398UL,2576877501UL,128555417UL,2576980376UL,2476054937UL,6710794UL,1621426182UL,1610614371UL,105907766UL,858784182UL,1348671491UL,1717764288UL,2166095926UL,1825357992UL,285358080UL,105464318UL,1069686855UL,2284340768UL,1082098752UL,1367367820UL,294133508UL,1717973004UL,404951050UL,114084960UL,3224376704UL,891288UL,808067143UL,2382371635UL,1350534208UL,2282865042UL,294129648UL,3775930890UL,404951071UL,130094176UL,2349007970UL,3288504729UL,857212424UL,90177971UL,1717973004UL,2282864643UL,114084874UL,3775931264UL,1885639711UL,66855428UL,142771298UL,3288856644UL,1159202312UL,3239608816UL,2791727872UL,25309440UL,7130310UL,2349007192UL,1879103897UL,855810564UL,148898227UL,621279300UL,142679065UL,2702737663UL,4262527520UL,25309441UL,545001798UL,2562732102UL,2353015193UL,858882144UL,3226861595UL,912679680UL,2827033600UL,7130304UL,4262527544UL,1191594305UL,541049376UL,1082665030UL,2353037188UL,72450144UL,202475551UL,174482992UL,1612194576UL,2147929292UL,2562732053UL,1191185817UL,858794528UL,1083047963UL,2454749060UL,4035449281UL,168921103UL,534843426UL,1612194576UL,1644675348UL,2576089860UL,147063449UL,3006470150UL,201515009UL,57042480UL,92803520UL,2147929292UL,534843427UL,74474634UL,1644428322UL,1141408068UL,147064824UL,4030896134UL,12654721UL,5662307UL,3321987633UL,3087035596UL,2576089856UL,74449113UL,3006464354UL,1141432321UL,421693432UL,4278747420UL,542185600UL,33427714UL,2785116721UL,1176534032UL,2576924720UL,1619804181UL,456339840UL,12594688UL,3565155UL,3227025692UL,939551948UL,2717782274UL,541525576UL,1176518594UL,2218821652UL,1619804287UL,520366464UL,806097288UL,268789350UL,3428849699UL,192939724UL,2576924720UL,541523981UL,456339478UL,2218823168UL,3247581311UL,267421713UL,570757512UL,270524688UL,174069795UL,73533377UL,1503235075UL,101237761UL,28521240UL,806093536UL,3221448294UL,3422914577UL,595592908UL,2317345040UL,570716260UL,1147274236UL,4165208193UL,101237767UL,2180006552UL,1660993816UL,822105702UL,3435528578UL,22544492UL,3650718723UL,2718199808UL,28521218UL,4165208288UL,471409927UL,2164197505UL,35692824UL,822214161UL,289800578UL,809902204UL,697931968UL,2153811008UL,1782577UL,4156441174UL,106976410UL,1610614368UL,1621516294UL,1717985382UL,1181091430UL,1360983997UL,2701639151UL,3312546782UL,730554019UL,2333391022UL,3803754027UL,3098421331UL,2436922338UL,1333519032UL,601814952UL,2667037834UL,4156434515UL,3186713236UL,4018885735UL,2085790397UL,3797231087UL,279555963UL,676297695UL,259776273UL,2078217000UL,3740838220UL,4156934723UL,3190771998UL,4049241847UL,2312402877UL,1185267183UL,4114476924UL,2848947272UL,1249632903UL,870258237UL,3742416362UL,4173195795UL,3318698206UL,763005175UL,1272207294UL,1587854833UL,4113952649UL,166689858UL,4017898741UL,2085418241UL,3802638735UL,364726395UL,2649373663UL,1867702008UL,2057238468UL,3571958927UL,2772314235UL,435250142UL,4044087029UL,2346644745UL,1459071087UL,2008893308UL,3186252770UL,4015251217UL,65202313UL,1293106666UL,2545763708UL,3186246628UL,1148997906UL,67958717UL,736032008UL,2077507921UL,2835760271UL,243850999UL,2178873258UL,1039093824UL,4017786661UL,3175106597UL,3766751479UL,2667640767UL,3762154233UL,1073749811UL,344281277UL,2862693877UL,4156465808UL,569342598UL,4017783192UL,1369383240UL,3743500495UL,558410299UL,65567711UL };

int LEDPin[] = { A5, A4, 13, 12,
         11, 10, 9, 8,
         7, 6, 5, 4,
         3, 2, 1, 0 };
int PlanePin[] = { A0, A1, A2, A3 };


void setup() {
    // DO NOT USE SERIAL BECAUSE PINS 0 AND 1 ARE USED FOR LEDS

    // Set up LED pins as output (active HIGH)
    for (int pin = 0; pin < PLANESIZE; pin++) {
        pinMode(LEDPin[pin], OUTPUT);
        digitalWrite(LEDPin[pin], LOW);
    }
    // Set up plane pins as outputs (active LOW)
    for (int pin = 0; pin < CUBESIZE; pin++) {
        pinMode(PlanePin[pin], OUTPUT);
        digitalWrite(PlanePin[pin], LOW);
    }
}
void loop() {
    unsigned long frames_count = getFrameCount(animation1);
    COMPRESSION compression_algo = getCompression(animation1);
    bool buffer_a[CUBESIZE * CUBESIZE * CUBESIZE];
    bool buffer_b[CUBESIZE * CUBESIZE * CUBESIZE];
    unsigned char metadata = 0;

    bool* previous_frame = buffer_a;
    bool* next_frame = buffer_b;

    // Loop over all frames of animation
    while (true) {
        unsigned long bits_offset = 0;

        for (unsigned long frame_id = 0; frame_id < frames_count; frame_id++) {
            bits_offset = readFrameDataAtOffset_P(animation1, bits_offset, next_frame, &metadata);

            if (frame_id != 0 && (compression_algo & COMPRESSION::FRAME_SUBTRACTION)) {
                // Reconstruct the next frame from the previous one
                addTwoFrames(previous_frame, next_frame);
            }

            unsigned long start_time = millis();
            unsigned long duration = (unsigned long)metadata * TIMECONST;
            while (millis() - start_time < duration) {
                showFrame(next_frame, metadata);
            }

            // Swap buffers
            bool* temp = previous_frame;
            previous_frame = next_frame;
            next_frame = temp;
        }
        break;
    }
}


void showFrame(bool* data, unsigned char metadata) {
    for (int plane = 0; plane < CUBESIZE; plane++) {
        // Turn previous plane off
        if (plane == 0) {
            digitalWrite(PlanePin[CUBESIZE - 1], LOW);
        } else {
            digitalWrite(PlanePin[plane - 1], LOW);
        }

        // Load current plane pattern data into ports
        int ledpin = 0;
        for (int ledrow = 0; ledrow < CUBESIZE; ledrow++) {
            for (int ledcol = 0; ledcol < CUBESIZE; ledcol++) {
                unsigned long index = (unsigned long)plane * CUBESIZE * CUBESIZE + ledrow * CUBESIZE + ledcol;
                digitalWrite(LEDPin[ledpin++], data[index] ? HIGH : LOW);
            }
        }

        // Turn current plane on
        digitalWrite(PlanePin[plane], HIGH);
        // Delay PLANETIME us
        delayMicroseconds(PLANETIME);
    }

    //String result = "";
    //for (int row = 0; row < CUBESIZE; row++) {
    //  for (int plane = 0; plane < CUBESIZE; plane++) {
    //    for (int col = 0; col < CUBESIZE; col++) {
    //      bool val = data[plane * CUBESIZE * CUBESIZE + row * CUBESIZE + col];
    //      result += val ? "1" : "0";
    //      result += " ";
    //    }
    //    result += "  ";
    //  }
    //  result += "\n";
    //}
    //Serial.println(result);
}