/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/

#include "internal_math_types.h"

/**
 * tangent look-up table:
 * tan(x) where 0 <= x <= PI / 2
 * riscv_dsp_tan_table_f32_4096[k] = tan(2 * PI * k / 4096)
 * where 0 <= k <= 1024.
 */

const float32_t riscv_dsp_tan_table_f32_4096[1025] =
{
    0.000000000000,     0.001533981991,     0.003067971201,     0.004601974850,
    0.006136000158,     0.007670054343,     0.009204144628,     0.010738278232,
    0.012272462380,     0.013806704292,     0.015341011194,     0.016875390311,
    0.018409848869,     0.019944394098,     0.021479033226,     0.023013773485,
    0.024548622109,     0.026083586333,     0.027618673396,     0.029153890537,
    0.030689244998,     0.032224744026,     0.033760394866,     0.035296204772,
    0.036832180995,     0.038368330793,     0.039904661426,     0.041441180157,
    0.042977894254,     0.044514810987,     0.046051937631,     0.047589281464,
    0.049126849769,     0.050664649834,     0.052202688948,     0.053740974408,
    0.055279513514,     0.056818313572,     0.058357381891,     0.059896725788,
    0.061436352582,     0.062976269599,     0.064516484170,     0.066057003633,
    0.067597835330,     0.069138986609,     0.070680464826,     0.072222277342,
    0.073764431522,     0.075306934742,     0.076849794382,     0.078393017828,
    0.079936612475,     0.081480585723,     0.083024944981,     0.084569697665,
    0.086114851198,     0.087660413009,     0.089206390539,     0.090752791232,
    0.092299622544,     0.093846891937,     0.095394606883,     0.096942774860,
    0.098491403357,     0.100040499871,     0.101590071908,     0.103140126982,
    0.104690672618,     0.106241716349,     0.107793265719,     0.109345328281,
    0.110897911596,     0.112451023238,     0.114004670789,     0.115558861842,
    0.117113604001,     0.118668904880,     0.120224772103,     0.121781213307,
    0.123338236137,     0.124895848252,     0.126454057320,     0.128012871024,
    0.129572297054,     0.131132343116,     0.132693016924,     0.134254326209,
    0.135816278709,     0.137378882179,     0.138942144384,     0.140506073101,
    0.142070676123,     0.143635961253,     0.145201936310,     0.146768609123,
    0.148335987538,     0.149904079413,     0.151472892620,     0.153042435045,
    0.154612714587,     0.156183739163,     0.157755516701,     0.159328055145,
    0.160901362453,     0.162475446601,     0.164050315575,     0.165625977381,
    0.167202440039,     0.168779711585,     0.170357800068,     0.171936713558,
    0.173516460138,     0.175097047907,     0.176678484983,     0.178260779498,
    0.179843939603,     0.181427973466,     0.183012889269,     0.184598695217,
    0.186185399528,     0.187773010439,     0.189361536206,     0.190950985101,
    0.192541365418,     0.194132685466,     0.195724953574,     0.197318178090,
    0.198912367380,     0.200507529830,     0.202103673847,     0.203700807854,
    0.205298940297,     0.206898079640,     0.208498234369,     0.210099412988,
    0.211701624024,     0.213304876022,     0.214909177551,     0.216514537199,
    0.218120963575,     0.219728465311,     0.221337051059,     0.222946729496,
    0.224557509317,     0.226169399242,     0.227782408013,     0.229396544394,
    0.231011817172,     0.232628235158,     0.234245807185,     0.235864542111,
    0.237484448816,     0.239105536205,     0.240727813207,     0.242351288775,
    0.243975971886,     0.245601871543,     0.247228996774,     0.248857356631,
    0.250486960191,     0.252117816559,     0.253749934863,     0.255383324259,
    0.257017993928,     0.258653953079,     0.260291210946,     0.261929776790,
    0.263569659900,     0.265210869593,     0.266853415211,     0.268497306127,
    0.270142551739,     0.271789161476,     0.273437144794,     0.275086511178,
    0.276737270140,     0.278389431226,     0.280043004006,     0.281697998084,
    0.283354423091,     0.285012288689,     0.286671604572,     0.288332380461,
    0.289994626113,     0.291658351310,     0.293323565871,     0.294990279644,
    0.296658502507,     0.298328244375,     0.299999515190,     0.301672324931,
    0.303346683607,     0.305022601262,     0.306700087972,     0.308379153847,
    0.310059809032,     0.311742063705,     0.313425928078,     0.315111412400,
    0.316798526953,     0.318487282054,     0.320177688057,     0.321869755350,
    0.323563494360,     0.325258915548,     0.326956029412,     0.328654846486,
    0.330355377344,     0.332057632596,     0.333761622887,     0.335467358906,
    0.337174851374,     0.338884111056,     0.340595148753,     0.342307975305,
    0.344022601592,     0.345739038536,     0.347457297096,     0.349177388274,
    0.350899323110,     0.352623112687,     0.354348768130,     0.356076300603,
    0.357805721315,     0.359537041515,     0.361270272495,     0.363005425593,
    0.364742512185,     0.366481543695,     0.368222531588,     0.369965487376,
    0.371710422613,     0.373457348899,     0.375206277880,     0.376957221246,
    0.378710190734,     0.380465198128,     0.382222255256,     0.383981373996,
    0.385742566271,     0.387505844053,     0.389271219362,     0.391038704266,
    0.392808310880,     0.394580051372,     0.396353937957,     0.398129982899,
    0.399908198515,     0.401688597169,     0.403471191279,     0.405255993313,
    0.407043015791,     0.408832271285,     0.410623772420,     0.412417531872,
    0.414213562373,     0.416011876707,     0.417812487712,     0.419615408281,
    0.421420651362,     0.423228229958,     0.425038157126,     0.426850445983,
    0.428665109699,     0.430482161503,     0.432301614680,     0.434123482572,
    0.435947778582,     0.437774516171,     0.439603708856,     0.441435370216,
    0.443269513891,     0.445106153579,     0.446945303040,     0.448786976095,
    0.450631186627,     0.452477948582,     0.454327275966,     0.456179182851,
    0.458033683371,     0.459890791724,     0.461750522175,     0.463612889050,
    0.465477906743,     0.467345589714,     0.469215952490,     0.471089009662,
    0.472964775891,     0.474843265907,     0.476724494505,     0.478608476553,
    0.480495226984,     0.482384760805,     0.484277093092,     0.486172238992,
    0.488070213723,     0.489971032576,     0.491874710915,     0.493781264177,
    0.495690707871,     0.497603057583,     0.499518328973,     0.501436537774,
    0.503357699799,     0.505281830935,     0.507208947147,     0.509139064477,
    0.511072199046,     0.513008367053,     0.514947584779,     0.516889868581,
    0.518835234900,     0.520783700257,     0.522735281255,     0.524689994580,
    0.526647856999,     0.528608885367,     0.530573096619,     0.532540507778,
    0.534511135951,     0.536484998332,     0.538462112201,     0.540442494928,
    0.542426163969,     0.544413136870,     0.546403431266,     0.548397064883,
    0.550394055537,     0.552394421138,     0.554398179686,     0.556405349275,
    0.558415948093,     0.560429994421,     0.562447506639,     0.564468503218,
    0.566493002730,     0.568521023842,     0.570552585321,     0.572587706030,
    0.574626404935,     0.576668701100,     0.578714613693,     0.580764161980,
    0.582817365335,     0.584874243231,     0.586934815248,     0.588999101069,
    0.591067120487,     0.593138893398,     0.595214439806,     0.597293779827,
    0.599376933682,     0.601463921705,     0.603554764340,     0.605649482143,
    0.607748095783,     0.609850626044,     0.611957093821,     0.614067520129,
    0.616181926095,     0.618300332967,     0.620422762108,     0.622549235004,
    0.624679773257,     0.626814398592,     0.628953132857,     0.631095998021,
    0.633243016178,     0.635394209545,     0.637549600469,     0.639709211419,
    0.641873064996,     0.644041183927,     0.646213591070,     0.648390309414,
    0.650571362080,     0.652756772323,     0.654946563529,     0.657140759223,
    0.659339383064,     0.661542458849,     0.663750010513,     0.665962062131,
    0.668178637919,     0.670399762235,     0.672625459577,     0.674855754592,
    0.677090672069,     0.679330236945,     0.681574474302,     0.683823409375,
    0.686077067545,     0.688335474347,     0.690598655468,     0.692866636748,
    0.695139444182,     0.697417103922,     0.699699642277,     0.701987085715,
    0.704279460865,     0.706576794516,     0.708879113621,     0.711186445295,
    0.713498816823,     0.715816255651,     0.718138789397,     0.720466445849,
    0.722799252964,     0.725137238872,     0.727480431878,     0.729828860461,
    0.732182553277,     0.734541539161,     0.736905847127,     0.739275506371,
    0.741650546272,     0.744030996392,     0.746416886479,     0.748808246471,
    0.751205106491,     0.753607496856,     0.756015448074,     0.758428990846,
    0.760848156070,     0.763272974841,     0.765703478451,     0.768139698394,
    0.770581666367,     0.773029414270,     0.775482974208,     0.777942378495,
    0.780407659654,     0.782878850418,     0.785355983733,     0.787839092761,
    0.790328210879,     0.792823371684,     0.795324608993,     0.797831956843,
    0.800345449499,     0.802865121449,     0.805391007411,     0.807923142331,
    0.810461561389,     0.813006299999,     0.815557393810,     0.818114878711,
    0.820678790829,     0.823249166535,     0.825826042444,     0.828409455420,
    0.830999442572,     0.833596041263,     0.836199289109,     0.838809223981,
    0.841425884007,     0.844049307578,     0.846679533346,     0.849316600226,
    0.851960547404,     0.854611414332,     0.857269240737,     0.859934066620,
    0.862605932257,     0.865284878205,     0.867970945304,     0.870664174676,
    0.873364607734,     0.876072286177,     0.878787251998,     0.881509547485,
    0.884239215225,     0.886976298104,     0.889720839312,     0.892472882345,
    0.895232471007,     0.897999649415,     0.900774462001,     0.903556953512,
    0.906347169019,     0.909145153914,     0.911950953917,     0.914764615075,
    0.917586183771,     0.920415706721,     0.923253230982,     0.926098803950,
    0.928952473370,     0.931814287334,     0.934684294283,     0.937562543018,
    0.940449082695,     0.943343962832,     0.946247233315,     0.949158944396,
    0.952079146701,     0.955007891230,     0.957945229365,     0.960891212868,
    0.963845893891,     0.966809324974,     0.969781559053,     0.972762649461,
    0.975752649932,     0.978751614609,     0.981759598042,     0.984776655196,
    0.987802841452,     0.990838212614,     0.993882824914,     0.996936735011,
    1.000000000000,     1.003072677414,     1.006154825229,     1.009246501870,
    1.012347766211,     1.015458677584,     1.018579295781,     1.021709681061,
    1.024849894150,     1.027999996252,     1.031160049049,     1.034330114707,
    1.037510255880,     1.040700535720,     1.043901017873,     1.047111766492,
    1.050332846240,     1.053564322292,     1.056806260343,     1.060058726615,
    1.063321787858,     1.066595511358,     1.069879964943,     1.073175216986,
    1.076481336415,     1.079798392714,     1.083126455931,     1.086465596684,
    1.089815886166,     1.093177396152,     1.096550199005,     1.099934367680,
    1.103329975733,     1.106737097327,     1.110155807236,     1.113586180854,
    1.117028294199,     1.120482223922,     1.123948047315,     1.127425842311,
    1.130915687499,     1.134417662126,     1.137931846106,     1.141458320025,
    1.144997165153,     1.148548463444,     1.152112297549,     1.155688750824,
    1.159277907333,     1.162879851860,     1.166494669912,     1.170122447734,
    1.173763272311,     1.177417231376,     1.181084413424,     1.184764907715,
    1.188458804283,     1.192166193946,     1.195887168316,     1.199621819802,
    1.203370241627,     1.207132527831,     1.210908773281,     1.214699073683,
    1.218503525588,     1.222322226404,     1.226155274404,     1.230002768737,
    1.233864809437,     1.237741497433,     1.241632934560,     1.245539223568,
    1.249460468134,     1.253396772870,     1.257348243337,     1.261314986055,
    1.265297108511,     1.269294719174,     1.273307927505,     1.277336843966,
    1.281381580037,     1.285442248222,     1.289518962065,     1.293611836161,
    1.297720986167,     1.301846528816,     1.305988581929,     1.310147264429,
    1.314322696351,     1.318514998859,     1.322724294256,     1.326950706000,
    1.331194358716,     1.335455378213,     1.339733891494,     1.344030026772,
    1.348343913487,     1.352675682315,     1.357025465191,     1.361393395318,
    1.365779607182,     1.370184236574,     1.374607420599,     1.379049297696,
    1.383510007653,     1.387989691624,     1.392488492146,     1.397006553156,
    1.401544020007,     1.406101039488,     1.410677759840,     1.415274330775,
    1.419890903494,     1.424527630706,     1.429184666647,     1.433862167098,
    1.438560289407,     1.443279192507,     1.448019036936,     1.452779984859,
    1.457562200087,     1.462365848099,     1.467191096063,     1.472038112859,
    1.476907069099,     1.481798137150,     1.486711491159,     1.491647307074,
    1.496605762665,     1.501587037555,     1.506591313237,     1.511618773102,
    1.516669602463,     1.521743988582,     1.526842120694,     1.531964190034,
    1.537110389862,     1.542280915494,     1.547475964324,     1.552695735858,
    1.557940431737,     1.563210255768,     1.568505413955,     1.573826114524,
    1.579172567960,     1.584544987032,     1.589943586825,     1.595368584777,
    1.600820200703,     1.606298656835,     1.611804177851,     1.617336990912,
    1.622897325693,     1.628485414423,     1.634101491913,     1.639745795601,
    1.645418565583,     1.651120044653,     1.656850478339,     1.662610114943,
    1.668399205584,     1.674218004229,     1.680066767744,     1.685945755929,
    1.691855231562,     1.697795460442,     1.703766711433,     1.709769256509,
    1.715803370796,     1.721869332622,     1.727967423562,     1.734097928485,
    1.740261135605,     1.746457336525,     1.752686826295,     1.758949903455,
    1.765246870094,     1.771578031898,     1.777943698206,     1.784344182064,
    1.790779800282,     1.797250873492,     1.803757726200,     1.810300686853,
    1.816880087892,     1.823496265820,     1.830149561257,     1.836840319009,
    1.843568888129,     1.850335621986,     1.857140878329,     1.863985019358,
    1.870868411789,     1.877791426933,     1.884754440758,     1.891757833971,
    1.898801992089,     1.905887305515,     1.913014169618,     1.920182984810,
    1.927394156630,     1.934648095822,     1.941945218424,     1.949285945848,
    1.956670704974,     1.964099928233,     1.971574053701,     1.979093525190,
    1.986658792343,     1.994270310733,     2.001928541955,     2.009633953731,
    2.017387020012,     2.025188221078,     2.033038043649,     2.040936980993,
    2.048885533031,     2.056884206457,     2.064933514850,     2.073033978791,
    2.081186125983,     2.089390491374,     2.097647617284,     2.105958053528,
    2.114322357549,     2.122741094551,     2.131214837634,     2.139744167932,
    2.148329674758,     2.156971955741,     2.165671616982,     2.174429273199,
    2.183245547884,     2.192121073458,     2.201056491434,     2.210052452577,
    2.219109617078,     2.228228654720,     2.237410245054,     2.246655077580,
    2.255963851929,     2.265337278049,     2.274776076397,     2.284280978134,
    2.293852725323,     2.303492071138,     2.313199780069,     2.322976628134,
    2.332823403101,     2.342740904713,     2.352729944908,     2.362791348063,
    2.372925951227,     2.383134604366,     2.393418170615,     2.403777526535,
    2.414213562373,     2.424727182331,     2.435319304841,     2.445990862847,
    2.456742804091,     2.467576091411,     2.478491703040,     2.489490632914,
    2.500573890994,     2.511742503586,     2.522997513673,     2.534339981258,
    2.545770983711,     2.557291616128,     2.568902991694,     2.580606242063,
    2.592402517738,     2.604292988468,     2.616278843653,     2.628361292757,
    2.640541565732,     2.652820913457,     2.665200608185,     2.677681943998,
    2.690266237280,     2.702954827200,     2.715749076209,     2.728650370544,
    2.741660120751,     2.754779762224,     2.768010755750,     2.781354588077,
    2.794812772490,     2.808386849411,     2.822078387004,     2.835888981808,
    2.849820259377,     2.863873874948,     2.878051514119,     2.892354893545,
    2.906785761666,     2.921345899435,     2.936037121087,     2.950861274915,
    2.965820244077,     2.980915947417,     2.996150340320,     3.011525415584,
    3.027043204318,     3.042705776869,     3.058515243776,     3.074473756746,
    3.090583509666,     3.106846739643,     3.123265728071,     3.139842801734,
    3.156580333941,     3.173480745693,     3.190546506892,     3.207780137577,
    3.225184209206,     3.242761345973,     3.260514226169,     3.278445583581,
    3.296558208938,     3.314854951406,     3.333338720116,     3.352012485763,
    3.370879282231,     3.389942208292,     3.409204429346,     3.428669179220,
    3.448339762033,     3.468219554112,     3.488312005977,     3.508620644391,
    3.529149074478,     3.549900981910,     3.570880135173,     3.592090387903,
    3.613535681307,     3.635220046665,     3.657147607917,     3.679322584343,
    3.701749293331,     3.724432153251,     3.747375686420,     3.770584522182,
    3.794063400088,     3.817817173201,     3.841850811510,     3.866169405479,
    3.890778169713,     3.915682446773,     3.940887711121,     3.966399573217,
    3.992223783770,     4.018366238147,     4.044832980952,     4.071630210779,
    4.098764285146,     4.126241725622,     4.154069223156,     4.182253643607,
    4.210802033503,     4.239721626021,     4.269019847209,     4.298704322457,
    4.328782883234,     4.359263574094,     4.390154659978,     4.421464633810,
    4.453202224414,     4.485376404763,     4.517996400574,     4.551071699274,
    4.584612059343,     4.618627520066,     4.653128411707,     4.688125366130,
    4.723629327882,     4.759651565783,     4.796203685015,     4.833297639778,
    4.870945746500,     4.909160697670,     4.947955576300,     4.987343871060,
    5.027339492126,     5.067956787771,     5.109210561757,     5.151116091546,
    5.193689147407,     5.236946012447,     5.280903503628,     5.325578993827,
    5.370990435004,     5.417156382524,     5.464096020733,     5.511829189836,
    5.560376414162,     5.609758931917,     5.659998726485,     5.711118559407,
    5.763142005119,     5.816093487567,     5.869998318822,     5.924882739822,
    5.980773963371,     6.037700219555,     6.095690803728,     6.154776127241,
    6.214987771089,     6.276358542696,     6.338922536033,     6.402715195306,
    6.467773382469,     6.534135448829,     6.601841311033,     6.670932531751,
    6.741452405415,     6.813446049356,     6.886960500772,     6.962044819944,
    7.038750200189,     7.117130085055,     7.197240293339,     7.279139152525,
    7.362887641324,     7.448549542044,     7.536191603586,     7.625883715956,
    7.717699097231,     7.811714494044,     7.908010396747,     8.006671270494,
    8.107785803677,     8.211447175212,     8.317753342395,     8.426807351191,
    8.538717671007,     8.653598556260,     8.771570437256,     8.892760343195,
    9.017302360425,     9.145338129429,     9.277017384405,     9.412498539764,
    9.551949328382,     9.695547497013,     9.843481564907,     9.995951652472,
    10.153170387609,    10.315363898374,    10.482772901697,    10.655653899192,
    10.834280492552,    11.018944832699,    11.209959218864,    11.407657865978,
    11.612398861435,    11.824566335310,    12.044572871723,    12.272862193180,
    12.509912154653,    12.756238089895,    13.012396559320,    13.278989556808,
    13.556669242352,    13.846143278886,    14.148180865204,    14.463619573287,
    14.793373117978,    15.138440210805,    15.499914678631,    15.878997063092,
    16.277007959935,    16.695403410578,    17.135792723905,    17.599959188084,
    18.089884234363,    18.607775743218,    19.156101345713,    19.737627779647,
    20.355467624987,    21.013135084857,    21.714612922734,    22.464433248714,
    23.267775617124,    24.130586923602,    25.059728970732,    26.063161449433,
    27.150170665700,    28.331657935180,    29.620506630823,    31.032054101288,
    32.584705164869,    34.300739338346,    36.207387142123,    38.338286277758,
    40.735483872083,    43.452239618953,    46.557030267474,    50.139402335560,
    54.318751180269,    59.257888651956,    65.184751340932,    72.428580988538,
    81.483240206546,    93.124798813181,   108.646706505164,   130.377172736208,
    162.972616413249,   217.298014985176,   325.948300797684,   651.898135577394,
    4095.000000000000
};
