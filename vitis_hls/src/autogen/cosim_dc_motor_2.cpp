#include "../mpc/generic_dense_cosim.hpp"

const int __cosim_iters = 99;
const std::vector<std::vector<float>> __cosim_x0 = {{0.8,-0.4}};
const std::vector<std::vector<float>> __cosim_x = {{0.7984,-0.37941229285573447},{0.7968823508285771,-0.3605937640548902},{0.7954399757723575,-0.34339139285767073},{0.7940664102009268,-0.3276653945027818},{0.7927557486229158,-0.31328807532189634},{0.7915025963216282,-0.3001427868844166},{0.7903020251740905,-0.28812297060661385},{0.789149533291664,-0.27713128500015666},{0.7880410081516634,-0.2670788084118881},{0.7869726929180159,-0.25788431072500967},{0.7859411556751158,-0.24947358805664985},{0.7849432613228892,-0.2417788550027554},{0.7839761459028781,-0.2347381894525785},{0.7830371931450678,-0.22829502542559518},{0.7821240130433654,-0.22239768977701085},{0.7812344222842574,-0.21699897897730885},{0.7803664263683482,-0.21205577249951732},{0.7795182032783501,-0.20752867964770066},{0.7786880885597592,-0.20338171693407694},{0.777874561692023,-0.19958201336236592},{0.7770762336385735,-0.19609954120353554},{0.7762918354737594,-0.19290687005890522},{0.7755202079935237,-0.1899789421962964},{0.7747602922247385,-0.18729286731915473},{0.7740111207554619,-0.18482773508773057},{0.7732718098151109,-0.18256444385680085},{0.7725415520396837,-0.18048554422723367},{0.7718196098627748,-0.1785750961300282},{0.7711053094782546,-0.1768185382722979},{0.7703980353251654,-0.17520256887591384},{0.7696972250496618,-0.17371503673201577},{0.7690023649027337,-0.17234484167908898},{0.7683129855360173,-0.17108184368948753},{0.7676286581612594,-0.16991677981979042},{0.7669489910419802,-0.1688411883447853},{0.7662736262886011,-0.16784733945370983},{0.7656022369307862,-0.16692817194112916},{0.7649345242430217,-0.166077235373925},{0.764270215301526,-0.1652886372607242},{0.763609060752483,-0.16455699479106536},{0.7629508327733188,-0.16387739074903065},{0.7622953232103227,-0.16324533324025986},{0.7616423418773617,-0.16265671890249672},{0.7609917150017517,-0.16210779929834926},{0.7603432838045583,-0.16159515021500853},{0.7596969032036982,-0.16111564361948016},{0.7590524406292203,-0.16066642203963208},{0.7584097749410618,-0.1602448751612299},{0.7577687954404169,-0.1598486184492819},{0.7571294009666197,-0.1594754736185946},{0.7564914990721454,-0.15912345079358609},{0.7558550052689711,-0.15879073221123957},{0.7552198423401262,-0.15847565733371877},{0.7545859397107914,-0.15817670924871208},{0.7539532328737965,-0.15789250224611964},{0.753321662864812,-0.15762177046933212},{0.7526911757829347,-0.15736335754815087},{0.752061722352742,-0.15711620712843982},{0.7514332575242283,-0.15687935422094335},{0.7508057401073445,-0.1566519172984141},{0.7501791324381508,-0.15643309107632325},{0.7495534000738455,-0.15622213991802497},{0.7489285115141734,-0.1560183918103607},{0.7483044379469319,-0.15582123286036179},{0.7476811530154904,-0.15563010226797605},{0.7470586326064185,-0.15544448773364358},{0.7464368546554839,-0.15526392126310778},{0.7458157989704315,-0.15508797533510216},{0.7451954470690911,-0.15491625940052406},{0.744575782031489,-0.15474841668442324},{0.7439567883647513,-0.15458412126461174},{0.7433384518796928,-0.1544230754029682},{0.7427207595780809,-0.15426500710757907},{0.7421036995496506,-0.15410966790574954},{0.7414872608780276,-0.15395683080964473},{0.740871433554789,-0.1538062884578986},{0.7402562084009574,-0.15365785141797017},{0.7396415769952855,-0.15351134663534238},{0.7390275316087441,-0.15336661601686213},{0.7384140651446767,-0.15322351513661867},{0.7378011710841302,-0.15308191205376065},{0.7371888434359152,-0.15294168623257004},{0.7365770766909849,-0.15280272755594698},{0.7359658657807612,-0.15266493542422654},{0.7353552060390642,-0.15252821793194601},{0.7347450931673365,-0.1523924911158199},{0.7341355232028732,-0.1522576782677639},{0.7335264924898022,-0.1521237093073406},{0.7329179976525728,-0.1519905202084877},{0.7323100355717389,-0.15185805247583273},{0.7317026033618356,-0.1517262526663058},{0.7310956983511704,-0.15159507195213162},{0.7304893180633618,-0.15146446572162203},{0.7298834602004753,-0.15133439321449907},{0.7292781226276174,-0.1512048171887618},{0.7286733033588624,-0.1510757036163687},{0.7280690005443969,-0.1509470214052427},{0.7274652124587759,-0.15081874214532215},{0.7268619374901947,-0.15069083987657822}};
const std::vector<std::vector<float>> __cosim_u = {{-0.6078959522401128},{-0.6475624056204657},{-0.6837213073106513},{-0.7166761168955836},{-0.7467040452633276},{-0.77405832506467},{-0.7989702847825033},{-0.8216512433983748},{-0.8422942411741838},{-0.8610756207247455},{-0.8781564713307488},{-0.8936839483215521},{-0.9077924783340138},{-0.9206048603188581},{-0.9322332713122037},{-0.9427801852098723},{-0.9523392120695711},{-0.9609958648151188},{-0.9688282596222991},{-0.9759077557227442},{-0.9822995398660695},{-0.9880631602272049},{-0.993253014131819},{-0.9979187935944728},{-1.0021058923186144},{-1.005855777491899},{-1.0092063294219555},{-1.0121921517943444},{-1.0148448550938238},{-1.0171933155102466},{-1.0192639114496196},{-1.0210807395874268},{-1.0226658122337795},{-1.0240392376268725},{-1.0252193846314228},{-1.026223033191022},{-1.0270655117666634},{-1.0277608228871122},{-1.0283217578394204},{-1.0287600014389444},{-1.0290862277369643},{-1.0293101874497903},{-1.02944078782542},{-1.0294861656018979},{-1.02945375365492},{-1.0293503418805536},{-1.0291821328117252},{-1.0289547924239952},{-1.0286734965467317},{-1.0283429732598155},{-1.0279675416231124},{-1.0275511470559229},{-1.0270973936561838},{-1.026609573724122},{-1.026090694732171},{-1.0255435039620457},{-1.0249705110107556},{-1.0243740083499},{-1.0237560901066183},{-1.0231186692200311},{-1.0224634931136773},{-1.0217921580123273},{-1.0211061220204083},{-1.0204067170691815},{-1.0196951598305066},{-1.0189725616865923},{-1.0182399378373794},{-1.0174982156201553},{-1.0167482421095375},{-1.0159907910600772},{-1.015226569248336},{-1.0144562222663902},{-1.0136803398142076},{-1.0128994605342423},{-1.0121140764278491},{-1.0113246368896862},{-1.0105315523931484},{-1.0097351978570217},{-1.0089359157209246},{-1.008134018754732},{-1.0073297926249904},{-1.006523498239342},{-1.0057153738881595},{-1.0049056372009375},{-1.0040944869334567},{-1.0032821046003626},{-1.0024686559665308},{-1.0016542924094303},{-1.0008391521636468},{-1.0000233614577556},{-0.9992070355528583},{-0.998390279691288},{-0.9975731899632525},{-0.9967558540985161},{-0.9959383521896019},{-0.9951207573524364},{-0.994303136329854},{-0.9934855500428956},{-0.992668054094421}};
const std::vector<std::vector<float>> __cosim_yref = {{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0},{0.0}};