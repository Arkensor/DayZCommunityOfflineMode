class PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	autoptr map< string, vector > Positions = new map< string, vector >;

    protected bool m_bOverCurrentPos;

	void PositionMenu()
	{
	    m_bOverCurrentPos = false;

        string currWorld = GetGame().GetWorldName();
        currWorld.ToLower();

        switch(currWorld)
        {
            case "chernarusplus":
                DefaultChernarusPlus();
            break;
            case "enoch":
                DefaultEnoch();
            break;
            case "namalsk":
                DefaultNamalsk();
            break;
            case "deerisle":
                DefaultDeerIsle();
            break;
            case "chiemsee":
                DefaultChiemsee();
            break;
            case "takistanplus":
                DefaultTakistanPlus();
            break;
            case "esseker":
                DefaultEsseker();
            break;
            case "rostow":
                DefaultRostow();
            break;
        }
	}

    void DefaultEnoch()
    {
        Positions.Insert( "Adamow", "3175.23 0 6805.85" );
        Positions.Insert( "Bagno", "11970.94 0 11220.80" );
        Positions.Insert( "Bielawa", "1392.87 0 9664.53" );
        Positions.Insert( "Borek", "9768.98 0 8497.65" );
        Positions.Insert( "Branzow Castle", "1075.98 0 11365.36" );
        Positions.Insert( "Brena", "6641.84 0 11130.16" );
        Positions.Insert( "Dambog", "554.79 0 1127.55" );
        Positions.Insert( "Drewniki", "5790.12 0 5087.65" );
        Positions.Insert( "Dolnik", "11390.37 0 612.61" );
        Positions.Insert( "Gieraltow", "11345.83 0 4397.68" );
        Positions.Insert( "Gliniska", "4993.88 0 9916.66" );
        Positions.Insert( "Grabinskie", "10959.82 0 10998.11" );
        Positions.Insert( "Hedrykow", "4507.86 0 4803.22" );
        Positions.Insert( "Hrud", "6479.31 0 9235.63" );
        Positions.Insert( "Huta", "5228.30 0 5567.43" );
        Positions.Insert( "Jantar", "3668.64 0 8889.91" );
        Positions.Insert( "Jezurow", "2104.29 0 2330.90" );
        Positions.Insert( "Karlin", "10078.41 0 6885.64" );
        Positions.Insert( "Kolembrody", "8417.17 0 11968.60" );
        Positions.Insert( "Kopa", "5555.70 0 8724.25" );
        Positions.Insert( "Krsnik", "7813.15 0 10044.03" );
        Positions.Insert( "Kulno", "11194.24 0 2453.57" );
        Positions.Insert( "Lipina", "5937.02 0 6777.02" );
        Positions.Insert( "Lukow", "3755.38 0 11720.90" );
        Positions.Insert( "Lembork", "8837.56 0 6614.49" );
        Positions.Insert( "Muratyn", "4586.78 0 6385.54" );
        Positions.Insert( "Nadbor", "6171.44 0 4091.37" );
        Positions.Insert( "Nidek", "6291.00 0 8077.26" );
        Positions.Insert( "Olszanka", "4757.12 0 7590.45" );
        Positions.Insert( "Piorun", "643.09 0 12108.47" );
        Positions.Insert( "Polana", "3362.77 0 2095.52" );
        Positions.Insert( "Radacz", "3962.44 0 7895.98" );
        Positions.Insert( "Radunin", "7291.96 0 6487.13" );
        Positions.Insert( "Rodzanica", "8900.85 0 2077.72" );
        Positions.Insert( "Roztoka", "7688.82 0 5238.29" );
        Positions.Insert( "Sitnickie", "11323.00 0 10018.24" );
        Positions.Insert( "Skala", "2997.42 0 1958.17" );
        Positions.Insert( "Sobotka", "6300.11 0 10161.06" );
        Positions.Insert( "Sowa", "11665.42 0 12014.70" );
        Positions.Insert( "Swarog", "4888.40 0 2211.34" );
        Positions.Insert( "Tarnow", "9268.88 0 10790.26" );
        Positions.Insert( "Topolin", "1873.00 0 7341.97" );
        Positions.Insert( "Tymbark", "2979.45 0 1129.89" );
        Positions.Insert( "Widok", "10256.57 0 2122.26" );
        Positions.Insert( "Wrzeszcz", "9037.59 0 4390.65" );
        Positions.Insert( "Zalesie", "865.18 0 5498.05" );
        Positions.Insert( "Zapadlisko", "8111.64 0 8693.00" );
    }

    void DefaultChiemsee()
    {
		Positions.Insert( "Herreninsel", "7027.956055 10.402502 3514.430664" );
		Positions.Insert( "Osternach", "3914.968506 29.584784 2703.943115" );
		Positions.Insert( "Baustelle", "4018.841553 7.807599 5078.954102" );
		Positions.Insert( "Hochstatt", "2623.289551 24.616463 5887.574707" );
		Positions.Insert( "Sagewerk", "4222.124512 12.522442 6658.396484" );
		Positions.Insert( "Gstadt Industrie", "8231.880859 4.878265 6337.642090" );
		Positions.Insert( "Gollenhausen", "9335.285156 6.277999 8797.252930" );
		Positions.Insert( "Eggstatt", "5296.567871 2.072500 8390.075195" );
		Positions.Insert( "Hemhof", "3345.000244 4.323145 8686.267578" );
		Positions.Insert( "Jolling", "1140.620239 19.302500 9485.746094" );
		Positions.Insert( "Soll", "8409.345703 45.072495 9785.223633" );
		Positions.Insert( "Stein", "4630.960449 74.067032 7582.009766" );
		Positions.Insert( "AltSchwanstein", "3954.993896 50.788231 1267.142090" );
		Positions.Insert( "Alm", "1405.421875 315.157776 1758.245361" );
		Positions.Insert( "Hub", "2136.849609 502.716614 538.320679" );
		Positions.Insert( "TedValley", "881.134155 27.349918 1320.070313" );
		Positions.Insert( "Fraueninsel", "8944.375977 16.812483 4821.572266" );
    }

    void DefaultNamalsk()
    {
		Positions.Insert( "Nemsk", "9043.1 9.31 10134.0" );
		Positions.Insert( "Jalovisko", "8177.9 16.12 10827.0" );
		Positions.Insert( "BK-T08", "3929.8 6.15 10010.0" );
		Positions.Insert( "Lubjansk", "4450.1 9.24 11238.0" );
		Positions.Insert( "BK-T07", "4414.0 20.94 10746" );
		Positions.Insert( "BK-M06", "4808.1 13.96 10848.0" );
		Positions.Insert( "Vorkuta", "6815.5 16.6 11306.0" );
		Positions.Insert( "Alakit Old Yard", "6153.5 36.43 10874.0" );
		Positions.Insert( "Alakit", "5766.0 16.86 10768.0" );
		Positions.Insert( "Sebjan Dam", "5707.3 22.05 9851.5" );
		Positions.Insert( "Sebjan Airfield", "6340.5 22.2 9342.2" );
		Positions.Insert( "Sebjan Factory East", "6481.0 16.45 9299.9" );
		Positions.Insert( "Tara Farcory", "7688.5 72.67 8724.1" );
		Positions.Insert( "Tara Hospital", "7302.3 84.83 7991.4" );
		Positions.Insert( "Tara Harbor", "7868.4 10.1 7715.9" );
		Positions.Insert( "Tara Rail Yard", "7671.7 42.05 7402.6" );
		Positions.Insert( "Tara", "7241.9 238.26 7026.8" );
		Positions.Insert( "BK-L16", "6004.3 20.3 6623.2" ); 
		Positions.Insert( "Sawmill", "7047.7 39.6 5823.5" );
		Positions.Insert( "Tara Bridge", "5913.0 41.0 5872.4" );
		Positions.Insert( "Brensk", "4350.5 5.03 4772.9" );
		Positions.Insert( "Brensk Factory", "4855.4 41.3 6230.7" );
		Positions.Insert( "Brensk Bridge", "4885.1 41.74 6082.0" );
		Positions.Insert( "Athena-2", "4980.0 47.7 6612.6" );
		Positions.Insert( "BK-L02", "3440.7 216.4 6706.1" );
		Positions.Insert( "C-130J Mohawk", "3180.1 117.31 7516.7" );
		Positions.Insert( "Athena-1", "3755.5 147.3 8243.6" );
		Positions.Insert( "Warehouses", "4686.5 23.63 8933.7" );
		Positions.Insert( "Sebjan Refugee Camp", "5170.5 74.13 8976.3" );
		Positions.Insert( "Sebjan Factory", "5817.3 43.99 8715.0" );
		Positions.Insert( "Sebjan", "5185.5 30.89 8535.1" );
		Positions.Insert( "Sebjan uranium Mine", "4792.9 50.648 8019.0" );
		Positions.Insert( "Athena Research institute", "4291.1 78.91 8049.2" );
		Positions.Insert( "Norinsk", "3919.5 50.44 7519.8" );
		Positions.Insert( "BK-L01", "4146.8 381.48 6573.7" );

    }

    void DefaultDeerIsle()
    {
		Positions.Insert( "Airfield", "5636.620605 74.012505 3661.829590" );
		Positions.Insert( "Stonigton", "6875.350098 38.832481 2005.443115" );
		Positions.Insert( "Paris Island", "2483.801270 22.462500 3354.750000" );
		Positions.Insert( "Westbrook", "3264.520752 22.368082 6273.124023" );
		Positions.Insert( "RCFI", "3779.281738 16.012457 9045.755859" );
		Positions.Insert( "Sandy Bay", "5472.219238 2.622500 7733.409180" );
		Positions.Insert( "Mountainville", "8639.942383 7.414711 8931.773438" );
		Positions.Insert( "Mount Katahdin", "7233.665039 601.595398 11729.587891" );
		Positions.Insert( "Race Circuit", "10016.242188 21.812498 10993.663086" );
		Positions.Insert( "Military Island", "13100.995117 12.922499 9738.477539" );
		Positions.Insert( "Devasted Swamps", "14350.466797 1.875131 6612.347656" );
		Positions.Insert( "Mine", "10933.339844 46.742496 2304.359131" );
		Positions.Insert( "Camo Bear", "8885.958008 22.195593 5981.856445" );
		Positions.Insert( "Oceanville", "10242.033203 41.685661 4880.735352" );
		Positions.Insert( "Smallvile", "10389.737305 9.088058 4355.288574" );
		Positions.Insert( "Camp Eagle", "8832.635742 45.432487 3794.614746" );
		Positions.Insert( "West Brook", "3262.582520 19.473124 6394.934082" );
		Positions.Insert( "Portland", "6046.375488 5.892501 14436.607422" );
    }

    void DefaultEsseker()
    {
        Positions.Insert( "Lower Esseker", "11003.58 0 5488.50" );
        Positions.Insert( "West Esseker", "8265.58 0 5616.65" );
        Positions.Insert( "Old Esseker", "8609.39 0 5361.92" );
        Positions.Insert( "Adela Airport", "10119.04 0 4925.90" );
        Positions.Insert( "Neptune Resort", "6473.06 0 5490.85" );
        Positions.Insert( "Kula", "5117.34 0 5165.01" );
        Positions.Insert( "Gulash", "6190.98 0 6212.86" );
        Positions.Insert( "Lug", "6350.38 0 6957.52" );
        Positions.Insert( "Posestra", "7602.95 0 6920.02" );
        Positions.Insert( "Tuk", "6970.03 0 9272.01" );
        Positions.Insert( "Pilana Sawmill", "9053.61 0 7897.93" );
        Positions.Insert( "Igman Military Base", "7684.22 0 10120.60" );
        Positions.Insert( "Borosh", "10101.85 0 9795.54" );
        Positions.Insert( "Gromada", "9922.91 0 10684.77" );
        Positions.Insert( "Kamen Quarry", "11145.01 0 9052.44" );
        Positions.Insert( "Novi Grad", "11706.05 0 7872.54" );
        Positions.Insert( "Chardak Resort", "10093.25 0 7892.07" );
    }

    void DefaultTakistanPlus()
    {
        Positions.Insert( "Laschkar_Radarbase", "11243.09 0 9108.92" );
        Positions.Insert( "Rasman Airbase", "5754.95 0 11372.86" );
        Positions.Insert( "LoyManara AirBase", "8059.4 0 1856.03" );
        Positions.Insert( "FeeruzAbad RadioTower", "4116.05 0 6754.08" );
        Positions.Insert( "FeeruzAbad MilitaryBase", "5151.2 0 6076.96" );
        Positions.Insert( "FeeruzAbad FOB", "5247.09 0 5796.93" );
        Positions.Insert( "Rustaq Military Base", "2632.2 0 5100.09" );
        Positions.Insert( "Garmsar Military Base", "10422.45 0 6434.39" );
        Positions.Insert( "Farakkah", "8861.27 0 5271.47" );
        Positions.Insert( "AzZablah", "11856.08 0 2612.1" );
        Positions.Insert( "AzZablah Outpost", "11874.14 0 2543.67" );
        Positions.Insert( "LoyManara Refinery ", "7115.06 0 1025.43" );
        Positions.Insert( "Behbaman", "6082.98 0 1144.36" );
        Positions.Insert( "Dhimran", "3606.67 0 8544.91" );
        Positions.Insert( "Al Falludiyah", "1496.45 0 3602.79" );
        Positions.Insert( "Zayda", "141.58 0 2795.6" );
        Positions.Insert( "Al Hamiqqah", "1563.4 0 5721.32" );
        Positions.Insert( "El Thoubaroh", "2039.67 0 7659.63" );
        Positions.Insert( "Marastar", "4146.03 0 11758.76" );
        Positions.Insert( "Al Babair", "10477.13 0 11086.94" );
        Positions.Insert( "Marmamsiyah", "12328.32 0 10408.79" );
        Positions.Insert( "Hammaqiyah", "11502.87 0 8238.27" );
        Positions.Insert( "Al Hajasra", "11152.05 0 7636.76" );
        Positions.Insert( "Qaikkah", "10200.38 0 2350.73" );
        Positions.Insert( "Chaman", "876.04 0 3099.05" );
        Positions.Insert( "Ad Dawi", "3238.29 0 3636.3" );
        Positions.Insert( "Qarma", "6792.44 0 6089.24" );
        Positions.Insert( "Al Falqas", "4217.27 0 10747.8" );
        Positions.Insert( "Al-Falqas Oil Fields", "3650.62 0 11103.28" );
        Positions.Insert( "Gospandi", "3933.13 0 9373.13" );
        Positions.Insert( "Bastam", "5622.66 0 8899.43" );
        Positions.Insert( "Karachinar", "12333.8 0 11110.64" );
        Positions.Insert( "Jalelam", "6832.69 0 12294.08" );
        Positions.Insert( "Mahahwit", "5670.26 0 9935.64" );
        Positions.Insert( "Garmarud", "9149.89 0 6716.39" );
        Positions.Insert( "Dodr", "5035.86 0 5428.01" );
        Positions.Insert( "FeeruzAbad", "5072 0 6158" );
        Positions.Insert( "Nagara", "3057 0 9808" );
        Positions.Insert( "Rasman", "6092 0 11018" );
        Positions.Insert( "Garmsar", "10603.58 0 6410.72" );
        Positions.Insert( "Nur", "1890.7 0 11867.49" );
        Positions.Insert( "Zavarak", "9951.81 0 11651.26" );
        Positions.Insert( "Falar", "6063.8 0 7256.97" );
        Positions.Insert( "Anar", "6030.72 0 5659.43" );
        Positions.Insert( "ChakChak", "4375.92 0 754.72" );
        Positions.Insert( "Sakhee", "3588.99 0 4284.47" );
        Positions.Insert( "LoyManara", "8622.05 0 2454.22" );
        Positions.Insert( "Mulladoost", "2009.21 0 7675.97" );
        Positions.Insert( "Jilavur", "2537.21 0 5026.47" );
        Positions.Insert( "Shukurkalay", "1529.11 0 3582.72" );
        Positions.Insert( "Imarat", "8238.39 0 7847.22" );
        Positions.Insert( "Ravanay", "11539.21 0 8318.97" );
        Positions.Insert( "Kakaru", "5313.46 0 4740.72" );
        Positions.Insert( "Landay", "1950.86 0 349.72" );
        Positions.Insert( "Huzrutimam", "5932.74 0 1245.22" );
        Positions.Insert( "Sultansafee", "6578.67 0 1955.22" );
        Positions.Insert( "Band-E Kirmiz", "3531.56 0 6884.1" );
        Positions.Insert( "Silsila-Ye Siah Kuh", "4812.84 0 2556.02" );
        Positions.Insert( "Reegshur Valley", "4246.14 0 5568.12" );
        Positions.Insert( "Lalezar Valley", "4401.77 0 9763.12" );
        Positions.Insert( "Darre-Ye Sabz", "8587.86 0 3204.21" );
        Positions.Insert( "Dawlatsah Valley", "7532.3 0 8283.12" );
        Positions.Insert( "Zabo Valley", "2774.33 0 8016.48" );
        Positions.Insert( "Dawlatsah Valley", "10104.34 0 6651.83" );
    }

    void DefaultRostow()
    {
        Positions.Insert( "Slowin", "1284 0 12693" );
        Positions.Insert( "Kolyzovka", "2757 0 13208" );
        Positions.Insert( "Bychau", "5858 0 13113" );
        Positions.Insert( "Domnowo Mili", "8744 0 13444" );
        Positions.Insert( "Domnowo Well", "8695 0 13286" );
        Positions.Insert( "Borowoje West", "11278 0 12630" );
        Positions.Insert( "Borowoje Industrial", "11820 0 12636" );
        Positions.Insert( "Alekssevka", "13802 0 11140" );
        Positions.Insert( "Ostrovez Industrial", "9617 0 10959" );
        Positions.Insert( "Ostrovez Central", "9582 0 10498" );
        Positions.Insert( "Ostrovez Airfield", "10562 0 10454" );
        Positions.Insert( "Adamowsk", "11659 0 8917" );
        Positions.Insert( "Dobrowo", "12793 0 8389" );
        Positions.Insert( "Olgowka", "13405 0 7153" );
        Positions.Insert( "Orschka", "6793 0 10908" );
        Positions.Insert( "Orschka Prison", "7912 0 4081" );
        Positions.Insert( "Orschka Castle", "7134 0 12382" );
        Positions.Insert( "Old Orschka", "5968 0 10572" );
        Positions.Insert( "Jelsk", "831 0 8370" );
        Positions.Insert( "Babrujsk", "4735 0 11390" );
        Positions.Insert( "Orlesko", "8070 0 7641" );
        Positions.Insert( "Orlesko Radio Tower", "7881 0 7909" );
        Positions.Insert( "Orlesko", "4735 0 11390" );
        Positions.Insert( "Rezoni", "9986 0 6781" );
        Positions.Insert( "Rezoni Sportsfacility", "10643 0 6607" );
        Positions.Insert( "Rezoni Castle", "9724 0 6692" );
        Positions.Insert( "Berasino", "3060 0 9363" );
        Positions.Insert( "Berasino Industrial", "3122 0 8566" );
        Positions.Insert( "Dribin", "4874 0 9259" );
        Positions.Insert( "Klettzk", "976 0 4025" );
        Positions.Insert( "Klettzk Old Mine", "1733 0 3047" );
        Positions.Insert( "Klettzk Radiotower", "634 0 2671" );
        Positions.Insert( "Krasnapolle South", "8590 0 1937" );
        Positions.Insert( "Krasnapolle North", "8943 0 2436" );
        Positions.Insert( "Krasnapolle West", "8641 0 2356" );
        Positions.Insert( "Prudnoje", "10192 0 1336" );
        Positions.Insert( "Mieva", "6793 0 10908" );
        Positions.Insert( "Mieva Airbase", "4521 0 5960" );
        Positions.Insert( "Mieva Caste", "2952 0 5630" );

    }

    void DefaultChernarusPlus()
    {
        Positions.Insert( "Altar", "8090.49 0 9326.95" );
        Positions.Insert( "Balota", "4393.72 0 2412.05" );
        Positions.Insert( "Balota Airstrip", "5025.65 0 2363.44" );
        Positions.Insert( "Bashnya", "4156.51 0 11747.29" );
        Positions.Insert( "Bay Mutnaya", "5634.24 0 1947.33" );
        Positions.Insert( "Bay Tikhaya", "1190.83 0 2194.76" );
        Positions.Insert( "Bay Zelenaya", "11140.75 0 3090.17" );
        Positions.Insert( "Belaya Polana", "14148.24 0 15004.23" );
        Positions.Insert( "Berezhki", "15152.52 0 13815.69" );
        Positions.Insert( "Berezino", "11856.29 0 8991.54" );
        Positions.Insert( "Berezino Lumberyard", "12661.27 0 9590.91" );
        Positions.Insert( "Berezino Shipyard", "13178.96 0 10213.62" );
        Positions.Insert( "Black Forest", "9083.59 0 7555.09" );
        Positions.Insert( "Black Lake", "13360.76 0 11902.36" );
        Positions.Insert( "Black Mtn", "10246.34 0 11984.03" );
        Positions.Insert( "Bogatyrka", "1504.85 0 8940.5" );
        Positions.Insert( "Bor", "3317.02 0 4014.26" );
        Positions.Insert( "Cap Golova", "8350.07 0 2443.64" );
        Positions.Insert( "Cernaya Polana", "12199.96 0 13666.45" );
        Positions.Insert( "Chernogorsk", "6572.9 0 2447.53" );
        Positions.Insert( "Chernogorsk Factory", "6406.17 0 2710.03" );
        Positions.Insert( "Chyornaya Bay", "7706.96 0 3087.25" );
        Positions.Insert( "Crown Castle", "1430 0 9218.07" );
        Positions.Insert( "Deep Lake", "1853.38 0 14860.82" );
        Positions.Insert( "Devil's Castle", "6909.76 0 11371.53" );
        Positions.Insert( "Dichina Military", "4622.19 0 8283.76" );
        Positions.Insert( "Dobroe", "13096.32 0 14953.18" );
        Positions.Insert( "Dolina", "11327.42 0 6645.57" );
        Positions.Insert( "Drakon Island", "11185.96 0 2491.77" );
        Positions.Insert( "Drozhino", "3446.32 0 4929.12" );
        Positions.Insert( "Dubky", "6653.59 0 3674.47" );
        Positions.Insert( "Dubrovka", "10397.6 0 9795.56" );
        Positions.Insert( "Dubrovka Vyshnaya", "9952.95 0 10392.8" );
        Positions.Insert( "Electrozavodsk", "10502.8 0 2336.88" );
        Positions.Insert( "Electrozavodsk Power", "10358.14 0 2593.85" );
        Positions.Insert( "Electrozavodsk Shipyard", "9961.48 0 1788.85" );
        Positions.Insert( "Gnome Castle", "7410.93 0 9114.04" );
        Positions.Insert( "Gorka", "9646.16 0 8810.25" );
        Positions.Insert( "Green Mtn", "3744.3 0 6013.15" );
        Positions.Insert( "Grishino", "6018.26 0 10256.4" );
        Positions.Insert( "Grozovoy Pass", "3237.3 0 15293.46" );
        Positions.Insert( "Guba", "14609.06 0 13283.4" );
        Positions.Insert( "Guglovo", "8411.8 0 6687.87" );
        Positions.Insert( "Gvozdno", "8706.38 0 11895.07" );
        Positions.Insert( "Kabanino", "5298.35 0 8563.76" );
        Positions.Insert( "Kamenka", "1892.27 0 2155.87" );
        Positions.Insert( "Kamensk", "6633.17 0 14466.59" );
        Positions.Insert( "Kamyshovo", "12170.79 0 3447.95" );
        Positions.Insert( "Karmanovka", "12519.8 0 14679.5" );
        Positions.Insert( "Khelm", "12286.8 0 10794.4" );
        Positions.Insert( "Klen Mtn", "11491.72 0 11331.18" );
        Positions.Insert( "Komarovo", "3619.86 0 2342.53" );
        Positions.Insert( "Kozlova Mtn", "8826.96 0 2858.41" );
        Positions.Insert( "Kozlovka", "4451.57 0 4577.18" );
        Positions.Insert( "Krasnoe", "6486.37 0 14981.87" );
        Positions.Insert( "Krasnostav", "11228.8 0 12223.2" );
        Positions.Insert( "Krasnostav Airstrip", "12016.22 0 12490.07" );
        Positions.Insert( "Krutoy Cap", "13624.71 0 3892.25" );
        Positions.Insert( "Kumyrna", "8403.05 0 6030.65" );
        Positions.Insert( "Lopatino", "2749.87 0 9991.8" );
        Positions.Insert( "Mamino", "7981.69 0 13057.3" );
        Positions.Insert( "Map Edge NE", "15321.19 0 15327.49" );
        Positions.Insert( "Map Edge NW", "44.12 0 15330.89" );
        Positions.Insert( "Map Edge SW", "19.82 0 1584.2" );
        Positions.Insert( "Mogilevka", "7502.8 0 5237.32" );
        Positions.Insert( "Msta", "11258.3 0 5511.28" );
        Positions.Insert( "Myshkino", "2056.57 0 7425.29" );
        Positions.Insert( "Myshkino Military", "1266.66 0 7225.99" );
        Positions.Insert( "Myshkino Trainyard", "885.72 0 7696.21" );
        Positions.Insert( "Nadezhdino", "5821.39 0 4764.33" );
        Positions.Insert( "Nagornoe", "9333.93 0 14666.87" );
        Positions.Insert( "Nizhnoye", "12895.4 0 8040.79" );
        Positions.Insert( "Novaya Petrovka", "3471.91 0 12988.1" );
        Positions.Insert( "Novaya Trainyard", "3526.6 0 12579.7" );
        Positions.Insert( "Novodmitrovsk", "11595 0 14696.5" );
        Positions.Insert( "Novoselky", "6052.29 0 3285.59" );
        Positions.Insert( "Novy Sobor", "7159.26 0 7688.51" );
        Positions.Insert( "NW Airfield ATC", "4618.3 0 10439.17" );
        Positions.Insert( "NW Airfield Range", "5373.21 0 9979.8" );
        Positions.Insert( "NW Airfield South", "4520.11 0 9421.26" );
        Positions.Insert( "NW Airfield Tents", "4163.31 0 11014.24" );
        Positions.Insert( "Olha", "13320.9 0 12893.05" );
        Positions.Insert( "Oreshka Pass", "9877.39 0 6009.26" );
        Positions.Insert( "Orlovets", "12136.28 0 7281.89" );
        Positions.Insert( "Orlovets Factory", "11489.7 0 7592.98" );
        Positions.Insert( "Otmel Island", "11642.89 0 3095.52" );
        Positions.Insert( "Pavlovo", "1626.37 0 3846.56" );
        Positions.Insert( "Pavlovo Military", "2101.77 0 3290.93" );
        Positions.Insert( "Pogorevka", "4376.71 0 6413.21" );
        Positions.Insert( "Polana", "10678.7 0 8090.18" );
        Positions.Insert( "Polesovo", "5817 0 13490" );
        Positions.Insert( "Prigorodki", "7984.89 0 3302.17" );
        Positions.Insert( "Prud Lake", "6627.34 0 9288.07" );
        Positions.Insert( "Pulkovo", "4902.36 0 5650.65" );
        Positions.Insert( "Pusta", "9176.44 0 3858.71" );
        Positions.Insert( "Pustoshka", "2992.21 0 7805.54" );
        Positions.Insert( "Ratnoe", "6305.55 0 12708.4" );
        Positions.Insert( "Rify", "13783.18 0 11233" );
        Positions.Insert( "Rog Castle", "11255 0 4293.29" );
        Positions.Insert( "Rogovo", "4731 0 6775.61" );
        Positions.Insert( "Romashka", "8148.5 0 11015.2" );
        Positions.Insert( "Schadenfreude Island", "24000 0 0" );
        Positions.Insert( "Severograd", "7893.4 0 12576.3" );
        Positions.Insert( "Shakhovka", "9670.8 0 6495.85" );
        Positions.Insert( "Silence Lake", "1203.95 0 6507.42" );
        Positions.Insert( "Sinystok", "1447.49 0 11924.24" );
        Positions.Insert( "Skalisty Castle", "13493.95 0 3254.96" );
        Positions.Insert( "Skalisty Island", "13661.17 0 2960.38" );
        Positions.Insert( "Skvsch Biathlon Arena", "488.57 0 11118.7" );
        Positions.Insert( "Smirnovo", "11629.3 0 15023.1" );
        Positions.Insert( "Solnichniy", "13469.1 0 6239" );
        Positions.Insert( "Solnichniy Factory", "13069.59 0 7161.34" );
        Positions.Insert( "Sosnovka", "2512.53 0 6371.41" );
        Positions.Insert( "Sosnovy Pass", "2736.13 0 6757.38" );
        Positions.Insert( "Staroye", "10164.18 0 5460.93" );
        Positions.Insert( "Stary Sobor", "6072.64 0 7748.56" );
        Positions.Insert( "Stary Yar", "4935.61 0 15068" );
        Positions.Insert( "Storozh Island (Prison)", "2611.2 0 1306.15" );
        Positions.Insert( "Svergino", "9533.2 0 13728.4" );
        Positions.Insert( "Svetlojarsk", "13931.93 0 13231.39" );
        Positions.Insert( "Three Crosses Gas", "312.94 0 9362.44" );
        Positions.Insert( "Three Valleys", "12842.1 0 5500.33" );
        Positions.Insert( "Tisy", "3413.75 0 14810.27" );
        Positions.Insert( "Tisy Military", "1681.3 0 14116.6" );
        Positions.Insert( "Topolka Dam", "10280.5 0 3507.5" );
        Positions.Insert( "Topolniki", "2821.19 0 12366.5" );
        Positions.Insert( "Troitskoe", "7559.44 0 13515.9" );
        Positions.Insert( "Troitskoe Military", "7866.4 0 14676.1" );
        Positions.Insert( "Tulga", "12808.7 0 4457.13" );
        Positions.Insert( "Turovo", "13576.5 0 14128.6" );
        Positions.Insert( "Vavilovo", "2211.83 0 11016.4" );
        Positions.Insert( "Veresnik Mtn", "4439.42 0 8078.14" );
        Positions.Insert( "Vybor", "3800 0 8844.25" );
        Positions.Insert( "Vyshnoye", "6532.61 0 6090.53" );
        Positions.Insert( "Vysotovo", "5616.26 0 2561.77" );
        Positions.Insert( "Willow Lake", "13254.2 0 11607.8" );
        Positions.Insert( "Zabolotye", "1156.26 0 10006.2" );
        Positions.Insert( "Zaprudnoe", "4927.47 0 13027.2" );
        Positions.Insert( "Zelenogorsk", "2713.4 0 5391.75" );
        Positions.Insert( "Zelenogorsk Trainyard", "2490.65 0 5151.76" );
        Positions.Insert( "Zub Castle", "6541.79 0 5591.69" );
        Positions.Insert( "Zvir", "477.35 0 5202.98" );
    }

	override void Init()
	{
		m_LstPositionList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_cancel") );

        for ( int nPosition = 0; nPosition < Positions.Count(); nPosition++ )
        {
            m_LstPositionList.AddItem( Positions.GetKey( nPosition ), NULL, 0 );
        }
	}

	override void OnShow()
	{
        vector player_pos = COM_GetPB().GetPosition();

        m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}

	override void OnHide()
	{

	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = true;
        }

		return false;
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = false;
        }

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		if ( m_bOverCurrentPos )
		{
            m_TxtSelectedX.SetText( "" );
            m_TxtSelectedY.SetText( "" );
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
		    float pos_x = 0;
            float pos_y = 0;

		    if( ( m_TxtSelectedX.GetText() != "" ) && ( m_TxtSelectedY.GetText() != "" ) )
		    {
	            pos_x = m_TxtSelectedX.GetText().ToFloat();
                pos_y = m_TxtSelectedY.GetText().ToFloat();
		    }
		    else if( ( m_TxtCurrentX.GetText() != "" ) && ( m_TxtCurrentY.GetText() != "" ) )
		    {
                pos_x = m_TxtCurrentX.GetText().ToFloat();
                pos_y = m_TxtCurrentY.GetText().ToFloat();
		    }
		    else
		    {
		        PlayerBase oPlayer = COM_GetPB();
		        oPlayer.MessageStatus( "Invalid teleportation position!" );

		        return true;
		    }

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = COM_SnapToGround( vPlayerPos );

			COM_GetPB().SetPosition( vPlayerPos );

			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";

		if( !Positions.Find( GetCurrentPositionName(), position ) )
		{
			position = "0 0 0";
		}

		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );

		return true;
	}

	string GetCurrentPositionName()
	{
		if ( m_LstPositionList.GetSelectedRow() != -1 )
		{
			string position_name;
			m_LstPositionList.GetItemText( m_LstPositionList.GetSelectedRow(), 0, position_name );
			return position_name;
		}

		return "";
	}
}