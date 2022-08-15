// Author - Aman-Rohilla@rohilla.co.in

/* 
Compilation Command - 
g++ -o spy2js.exe spy2js.cpp -std=c++17 -ljsoncpp -lshlwapi -lwsock32 -lwininet -I. -Iinclude -Llib

Use pyinstaller (pip install pyinstaller) to convert python_spy2js\flask_server.py into exe -> pyinstaller flask_server.py
*/

#include <utils.h>

size_t found = 0, pos=0, vsi=0, vsl, vscs=0, vsca=0;
vector<string> conv_songs, conv_albums, conversion;
string sf = "sources_spy2js\\";
string pf = "python_spy2js\\";

string albums_file       = sf+"albums.txt";
string songs_file        = sf+"songs.txt";
string covers_file       = sf+"covers.txt";
string js_covers_f       = sf+"covers\\jiosaavn\\";
string spy_covers_f      = sf+"covers\\spotify\\";
string temp_cover        = sf+"covers\\temp_cover__";
string album_jsons_f     = sf+"json\\";
string token_file        = "spotify_token";
string client_file       = "spotify_client";
string flask_server      = pf+"flask_server.py";
// string flask_server      = pf+"flask_server\\flask_server.exe";
string chrome_driver     = pf+"chromedriver.exe";
string chrome_exe        = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";
string curl_exe          = "C:\\Windows\\System32\\curl.exe";
string curl_params       = "--silent --max-time 30";
string spy_cover;
bool server_running = false;

int download_cover(string link, string cover)
{
    if(!exists(cover))
    {   
        int retval;
        pipe_open(curl_exe+" "+curl_params+" \""+link+"\" -o \""+temp_cover+"\"",&retval);
        if(retval)
        {
            del_file(temp_cover);
            return 0;
        }
        else MoveFileExA(temp_cover.c_str(),cover.c_str(),1|2);
    }
    return 1;
}

string remove_spchar(string s)
{
    string rs;
    int len = s.length();

    for(int i=0; i<len; i++)
        if((s[i]<='z' && s[i]>='a') || (s[i]<='Z' && s[i]>='A') || (s[i]<='9' && s[i]>='0'))
            rs.push_back(s[i]);

    rs = lower_case_converter(rs);
    return rs;
}

int parse_duration(string str) {
    size_t f = str.find(":");
    return 60*stoi(copy_str(str, 0, f-1)) + stoi(copy_str(str, f+1, str.length()-1));
}

void match_song_in_album(spy_tag__ tag, Json::Value ra, int index, progress_bar_param &pb, string * js_str=NULL)
{
    int js = ra["songs"].size();
    string url;

    string spy_title = tag.title;
    int val_at = valid_album_title(tag.album);
    if(val_at) spy_title = extract_title(tag.album);

    spy_title = remove_spchar(spy_title);
    int duration = stoi(tag.duration)/1000;
    string s, js_title, js_art;

    vector<string> spy_artists;
    separate_columns(tag.artists,spy_artists,',');
    sort(spy_artists.begin(),spy_artists.end());
    int vs_art = spy_artists.size();

    for(int j=0; j<js; j++)
    {
        js_title = ra["songs"][j]["song"].asString();

        if(val_at && valid_album_title(js_title))
        js_title = extract_title(js_title);
    
        js_title = remove_spchar(js_title);

        if(abs(parse_duration(ra["songs"][j]["duration"].asString()) - duration) < 5 && (str_find(js_title,spy_title) || str_find(spy_title,js_title)))
        {
            url = ra["songs"][j]["perma_url"].asString();
            int noam=0;
            vector<string> js_artists;
            separate_columns(replace_string_all(ra["songs"][j]["singers"].asString(),", ",","),js_artists,',');

            int vs_js_art=js_artists.size();

            for(int i=0; i<vs_js_art; i++)
                if(binary_search(spy_artists.begin(),spy_artists.end(),js_artists[i]))
                    noam++;

            if(noam==vs_art) break;
        }
    }

    if(url=="") return;
    conversion[index] = url;
    string album_url = ra["perma_url"].asString();

    write_to_text(songs_file,tag.url+"|"+url);

    conv_songs.push_back(tag.url+"|"+url);
    conv_albums.push_back(tag.album_url+"|"+album_url);
    vscs++;
    vsca++;
    psv_sorter_vec(conv_songs,1);
    psv_sorter_vec(conv_albums,1);

    string jf;

    if(js_str)    
        write_to_text(albums_file,tag.album_url+"|"+album_url);

    found++;
    if(pb.execute)
    {
        pb.extra_info = "Found : "+to_string(found);
        pb.update();
    }
}

void start_server() {
    if(server_running) return;
    string args = chrome_driver;
    string process = flask_server;
    if(ext_ex(process)==".py") {
        process = "python";
        args = flask_server + " " + args;
    }

    if(shell_execute(flask_server, chrome_driver, ".", 0) !=-1) {
        server_running = true;
        while("LISTENING" != pipe_open(curl_exe+" "+curl_params+" http://127.0.0.1:45632/"))
            Sleep(1);
    }
}

int main(int argc, char *argv[])
{
    argcg = argc;
    argvg = argv;

    if (argc == 2 && ((string)(argv[1]) == "-v" || (string)(argv[1]) == "--version")) {
        cout<<"1.0.0";
        return 0;
    }
    if(argc==2 && ((string)(argv[1]) == "-h" || (string)(argv[1]) == "--help"))
    {
        cout<<R"(
      Spotify Playlist To JioSaavn Song Links Converter By Aman-Rohilla@rohilla.co.in
_________________________________________________________________________________________

      Usage : spy2js.exe spotify-playlist-link
      Flags : out-file, failed-file, token, client, no-saving-token-client, debug, chrome-exe-path
      Pause/Resume : CTRL + SHIFT 
)";
    return 0;
    }



    string pid;
    string token;
    string client;
    bool save_token_client=true;
    if(argc==1) cout<<endl;

    cout<<R"(
      Spotify Playlist To JioSaavn Song Links Converter By Aman-Rohilla@rohilla.co.in
_________________________________________________________________________________________

)";

    set_console_title("Spotify -> JioSaavn");
    if(argc==1) {
        int x, y;
        current_console_cursor(x,y);
        cout<<"\tEnter Spotify playlist URl/URI : ";
        getline(cin, pid);
        cout<<"\tEnter Spotify access token otherwise skip : ";
        getline(cin, token);
        cout<<"\tEnter Spotify Client ID and Client Secret as ID:SECRET or base64 encoded otherwise skip if already provided : ";
        getline(cin, client);
        cout<<"\tEnter y/n to save the client/token for further use (default=y) in "<<
            ext_dn(exe_path())<<" folder : ";

        string s;
        getline(cin, s);
        if(s == "n" || s=="N") save_token_client=false;
        erase_console(x, y);
    } else {
        pid = argv[1];
        arg_value("--token", &token);
        arg_value("--client", &client);
        arg_value("--chrome-exe-path", &chrome_exe);
        save_token_client = !arg_find("--no-saving-token-client");
    }
    
    chrome_exe = abs_path(chrome_exe);
    chrome_exe = replace_string_all(chrome_exe, "\\", "\\\\");

    string cd = directory();
    cd += "\\";
    string sd = ext_dn(exe_path());
    SetCurrentDirectoryA(sd.c_str());

    if(!exists(chrome_exe)) exiting("\tChrome Browser was not found at "
    +chrome_exe+"\nProvide Chrome exe path in command line args as --chrome-exe-path EXE_PATH");

    string chrome_version = pipe_open("C:\\Windows\\System32\\wbem\\WMIC.exe datafile where name=\""+chrome_exe+"\" get Version /value");
    chrome_version = copy_str(chrome_version, 
        str_find_index(chrome_version,"=")+1, 
        str_find_index(chrome_version,".")-1);

    string chrome_driver_version_str = pipe_open(chrome_driver+" --version");
    string chrome_driver_version = copy_str(chrome_driver_version_str,
        str_find_index(chrome_driver_version_str, " ")+1,
        str_find_index(chrome_driver_version_str, ".")-1);

    if(chrome_version != chrome_driver_version) exiting("\tInstalled Chrome Brower and "+chrome_driver+" don't have save version...");

    progress_bar_param pb;
    pb.task = "\tProcessing : ";
    static pause_param pp(&pb);
    pp.title="Spotify To JioSaavn";

    if(!network_connection()) exiting("\tNETWORK DISCONNECTED...");
    
    atexit([]() {
        console_cursor(1);
        setConsoleColor(7);
        del_file(temp_cover);
        pp.exit;
        if(server_running) {
            pipe_open(curl_exe+" "+curl_params+" http://127.0.0.1:45632/kill-server/COMMAND_FROM_CPP");
        }
    });
    
    string s, t;
    size_t f;

    md_if(spy_covers_f);
    md_if(js_covers_f);
    md_if(album_jsons_f);

    if(!exists(spy_covers_f)) exiting("\t"+sd+" directory is not writable...");

    ////////// Setting Variables

    pp.delayed_message=1;
    int debug=arg_find("--debug");
    if(!debug) debug = arg_find("--chrome-search");
    if(debug) pb.execute=0;

    int status=0;
    int retval;

    vector<spy_tag__> tagv, query;

    string pln;

    if(token!="") {
        try {
            if(!spy_pl2tags(pid, tagv, token, pln)) exiting("\tFailed...");
        } catch (string error) {
            if(error=="AccessTokenExpired") exiting("\tERROR... access token has expired");
        }
        if(save_token_client) 
            token_client_saver(token_file, client_file, token, client);
    } else {
        token = get_last_line(token_file);
        if(token!="") {
            try {
                if(!spy_pl2tags(pid, tagv, token, pln)) exiting("\tFailed...");
            } catch (string error) {
                if(error=="AccessTokenExpired") {
                    try {
                        token = generate_and_save_spy_token(token_file, client_file, client, save_token_client);
                        if(!spy_pl2tags(pid, tagv, token, pln)) exiting("\tFailed...");
                    } catch (string error) {
                        if(error=="UnableToGetToken") 
                            exiting("\tUnable to retrieve access token, ERROR with Client ID/Secret...");
                    }
                }
            }
        } else {
            try {
                token = generate_and_save_spy_token(token_file, client_file, client, save_token_client);
                if(!spy_pl2tags(pid, tagv, token, pln)) exiting("\tFailed...");
            } catch (string error) {
                if(error=="UnableToGetToken") 
                    exiting("\tUnable to retrieve access token, ERROR with Client ID/Secret...");
            }
        }
    }

    if(client=="") client = get_last_line(client_file);
    if(!tagv.size()) exiting("\tPlaylist is EMPTY...");

    size_t tagvs = tagv.size();
    text_to_vector(songs_file,conv_songs);
    text_to_vector(albums_file,conv_albums);

    conversion.resize(tagvs);

    psv_sorter_vec(conv_songs,1);
    psv_sorter_vec(conv_albums,1);

    vscs=conv_songs.size();
    vsca=conv_albums.size();
    
    if(pp.execute) pp.launch();
    if(pb.execute) pb.total_count = tagvs;

    spy_tag__ tag;
    string jsf, json_str, errors, json_file, js_cover, q, js_art;
    string spy_album, js_album, spy_title, js_title, phs;
    
    int js=0;
    int duration;
    pb.set_len = 37;

    js_search_tag__ jst;

    for(pos=0; pos<tagvs; pos++)
    {
        vector<string> match, links;
        vector<js_search_tag__> sv;
        Json::Value r, rs;
        pp.resume();
        if(pb.execute)
        {
            pb.current_count++;
            pb.new_line = "\t" + tagv[pos].title+" - "+tagv[pos].album;
            pb.update();
        }
        
        tag = tagv[pos];
        if(!is_int(tag.duration)) continue;

        duration = stoi(tag.duration)/1000;

        f = binary_search_string_vec(conv_songs,0,vscs,tag.url,1);

        if(f!=-1)
        {
            conversion[pos] = get_column(conv_songs[f],2);
            found++;
            if(pb.execute)
            {
                pb.extra_info = "Found : "+to_string(found);
                pb.update();
            }
            continue;
        }

        f = binary_search_string_vec(conv_albums,0,vsca,tag.album_url,1);
        if(f!=-1)
        {
            Json::Value ra;
            json_str = text_to_string(album_jsons_f+erase_rfind(get_column(conv_albums[f],2),"/")+".json");
            str2json(json_str,ra);
            if(ra.isMember("albumid"))
            {
                match_song_in_album(tag,ra,pos,pb);
                continue;
            }
        }

        spy_album = tag.album;
        spy_title = tag.title;

        string spy_album_safe, spy_title_safe = spy_title;

        spy_album = replace_string_all(spy_album," (Original Motion Picture Soundtrack)","");
        spy_album = replace_string_all(spy_album," - Single","");
        spy_album = replace_string_all(spy_album," (OST)","");

        spy_album_safe = spy_album;
        t = spy_title;

        if(remove_spchar(spy_album)==remove_spchar(spy_title)) s = get_column(tag.artists,1,',');
        else if(str_find(spy_title," - ") && t==copy_str(spy_album,0,spy_title.length()-1))
        {
            t = spy_album;
            s = get_column(tag.artists,1,',');
        }
        else s = spy_album;

        q = t+"+"+s+"+"+erase_find_found_to_end(tag.date,"-");

        string search = q;
        string search_cd = "\""+t+"\" \""+s+"\" "+erase_find_found_to_end(tag.date,"-");
        search_cd = "https://www.jiosaavn.com/search/album/"+url_encoder(search_cd);
        if(debug) cout<<"\n\n "<<pos+1<<".\t      "<<spy_title<<"|"<<duration<<"|"<<spy_album<<"|"<<tag.date<<endl;
        if(debug) cout<<"\n\t      "<<search;

        q = "https://jiosaavn-api.vercel.app/albumsearch?query="+url_encoder(q);
        json_str = pipe_open(curl_exe+" "+curl_params+" \""+q+"\"", &retval);
        json_str = decode_html_chars(json_str,1);
        Json::Value ras;
        str2json(json_str,ras);
        ras = ras["results"];
        spy_cover = spy_covers_f+erase_rfind(tag.cover,"/")+".jpg";
        string spy_cover_2, spy_cover_url;
        string spy_year = erase_find_found_to_end(tag.date,"-");

        vector<string> scv, say; //spotify cover vector
        vector<js_tag__> jav;
        scv.push_back(tag.cover);
        say.push_back(erase_find_found_to_end(tag.date,"-"));

        if(valid_album_title(spy_album) && valid_album_title(spy_title))
        {
            t = erase_find_found_to_end(tag.date,"-");
            if(is_int(t)) t = "%20year%3A"+to_string(stoi(t)-1)+"-"+t;
            else t = "";
            
            json_str = pipe_open(curl_exe+" "+curl_params+" -X \"GET\" \"https://api.spotify.com/v1/search?q=isrc%3A"+tag.isrc+t+"&type=track&market=IN&limit=50\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer "+token+"\"");

            spy_album = extract_album(tag.album);
            spy_title = extract_title(tag.title);

            Json::Value rsi;
            str2json(json_str,rsi);
            if(rsi["error"]["message"]=="The access token expired") {
                if(client!="") {
                    try {
                        token = generate_and_save_spy_token(token_file, client_file, client, save_token_client);
                    } catch (string error) {
                        if(error=="UnableToGetToken") 
                            exiting("\tUnable to retrieve access token, ERROR with Client ID/Secret...");
                    }
                } else {
                    exiting("\tAccess token got expired while processing...");
                }
            }

            if(rsi.isMember("tracks"))
            {
                int vs_si = rsi["tracks"]["total"].asInt();
                
                if(vs_si!=1)
                {
                    for(int i=0; i<vs_si; i++)
                    {
                        s = rsi["tracks"]["items"][i]["album"]["name"].asString();
                        t = rsi["tracks"]["items"][i]["name"].asString();

                        if(valid_album_title(s) || valid_album_title(t)) continue;

                        if(spy_album!=copy_str(s,0,spy_album.length()-1) || !str_find(t,spy_title)) continue;

                        spy_cover_url = rsi["tracks"]["items"][i]["album"]["images"][0]["url"].asString();
                        spy_year = rsi["tracks"]["items"][i]["album"]["release_date"].asString();
                        spy_year = erase_find_found_to_end(spy_year,"-");
                        scv.push_back(spy_cover_url);
                        say.push_back(erase_find_found_to_end(rsi["tracks"]["items"][i]["album"]["release_date"].asString(),"-"));
                    }
                }
            }

            if(spy_album==spy_title) s = get_column(tag.artists,1,',');
            else s = spy_album;

            if(s!="") s += "+";
            q = spy_title+"+"+s+"+"+erase_find_found_to_end(tag.date,"-");
            q = url_encoder(q);
            q = "https://jiosaavn-api.vercel.app/albumsearch?query="+q;

            json_str = pipe_open(curl_exe+" "+curl_params+" \""+q+"\"");
            json_str = decode_html_chars(json_str,1);

            Json::Value r;
            str2json(json_str,r);
            r = r["results"];

            if(!ras.isArray() || !ras.size())
                ras = r;
            else if(r.isArray() && r.size()) {
                int vs = r.size();
                for(int i=0; i<vs; i++)
                ras.append(r[i]);
            }
        }

        if(retval) continue;

        if(ras.isArray() && ras.size())
        {
            js_tag__ t;
            int vs = ras.size();
            for(int i=0; i<vs; i++)
            {
                ras[i]["image"] = replace_string_all(ras[i]["image"].asString(),"150x150.jpg", "500x500.jpg");
                t.cover     = ras[i]["image"].asString();

                t.album     = ras[i]["title"].asString();
                t.album_url = ras[i]["url"].asString();
                jav.push_back(t);
            }
        }

        sort(say.begin(), say.end());

        int val_at, vs, vs_l, ff=0;

        vector<string> jav_t;
        int fff;
        string img_compare_res, res_success = "SUCCESS|", res_error = "ERROR";

        string spy_cover_cmd;
        s = scv[0];
        s = spy_covers_f+erase_rfind(s,"/")+".jpg";
        for(int i=1; i<scv.size(); i++)
            s = s + "|"+spy_covers_f+erase_rfind(scv[i],"/")+".jpg";
        spy_cover_cmd = s;

        for(auto itr=scv.begin(); itr!=scv.end(); ++itr)
        if(!download_cover(*itr,spy_covers_f+erase_rfind(*itr,"/")+".jpg")) goto for_end;

        vs_l = jav.size();

        //img_similarity_surf__ ims("");

        spy_title = remove_spchar(spy_title);
        spy_album = remove_spchar(spy_album);


        val_at = valid_album_title(tag.title) && valid_album_title(tag.album);

        if(debug && vs_l) cout<<endl;

        for(int i=0; i<vs_l; i++)
        {
            if(debug) cout<<"\n\t"<<add_gaps(to_string(i+1), 3, 1)<<".  "<<jav[i].album;

            js_album = jav[i].album;

            js_album = replace_string_all(js_album," (Original Motion Picture Soundtrack)","");
            js_album = replace_string_all(js_album," - Single","");
            js_album = replace_string_all(js_album," (OST)","");

            if(val_at) js_album = extract_album(js_album);
            js_album = remove_spchar(js_album);

            if(!str_find(js_album,spy_album) && !str_find(spy_album,js_album)) continue;

            if(debug) cout<<" ---- ";
            js_cover = js_covers_f+validate_path(erase_rfind(jav[i].cover,"/"));

            if(!download_cover(replace_string_all(jav[i].cover,"https://","https://i1.wp.com/"),js_cover)) goto for_end;
            start_server();
            img_compare_res = pipe_open(curl_exe+" "+curl_params+" \"http://127.0.0.1:45632/img-compare?paths="+spy_cover_cmd+"|"+js_cover+"\"");

            if(img_compare_res.find(res_success)==0) 
                fff = stoi(img_compare_res.erase(0, res_success.length()));
            
            if(fff>=0)
            {   
                spy_cover = spy_covers_f+erase_rfind(scv[fff],"/")+".jpg";
                Json::Value ra;
                string js_album_id = get_album_id(jav[i].album_url);
                if(js_album_id=="") goto for_end;
                int st = download_or_read_json("https://jiosaavn-api.vercel.app/album?id="+js_album_id, album_jsons_f+erase_rfind(jav[i].album_url,"/")+".json",ra,"albumid");

                if(!st) goto for_end;

                match_song_in_album(tag,ra,pos,pb,&json_str);

                if(debug) cout<<" ++++ ";
                goto for_end;
            }
        }

        for(int i=0; i<vs_l; i++)
        jav_t.push_back(jav[i].album_url);

        sort(jav_t.begin(), jav_t.end());
        jav.clear();


        if(debug) cout<<"   >>>>   Chrome Driver";
        if(arg_find("--chrome-search")) cout<<" - "<<erase_rfind(search_cd,"/");
        if(debug && !vs_l) cout<<endl; 
        start_server();
        s = pipe_open(curl_exe+" "+curl_params+" \"http://127.0.0.1:45632/search?url="+search_cd+"\"");
        js_ate(s,jav);

        if(!arg_find("--chrome-search"))
        for(auto itr=jav.begin(); itr!=jav.end(); ++itr)
        {
            if(binary_search(jav_t.begin(), jav_t.end(),itr->album_url))
            {
                jav.erase(itr);
                --itr;
            }
        }

        vs = jav.size();

        for(int i=0; i<vs; i++)
        {
            if(debug) cout<<"\n\t"<<add_gaps(to_string(i+vs_l+1), 3, 1)<<".  "<<jav[i].album;

            js_album = jav[i].album;

            js_album = replace_string_all(js_album," (Original Motion Picture Soundtrack)","");
            js_album = replace_string_all(js_album," - Single","");
            js_album = replace_string_all(js_album," (OST)","");

            if(val_at) js_album = extract_album(js_album);
            js_album = remove_spchar(js_album);

            if(!str_find(js_album,spy_album) && !str_find(spy_album,js_album)) continue;

            Json::Value ra;
            string js_album_id = get_album_id(jav[i].album_url);
            if(js_album_id=="") goto for_end;
            int st = download_or_read_json("https://jiosaavn-api.vercel.app/album?id="+js_album_id, album_jsons_f+erase_rfind(jav[i].album_url,"/")+".json",ra,"albumid");

            if(!st) goto for_end;

            int fk=0;
            int nos = ra["songs"].size();
            for(int k=0; k<nos; k++)
            {
                s = remove_spchar(ra["songs"][k]["song"].asString());

                if(abs(parse_duration(ra["songs"][k]["duration"].asString()) - duration)<5 && (str_find(spy_title,s) || str_find(s,spy_title))
                 && binary_search(say.begin(), say.end(), erase_find_found_to_end(ra["songs"][k]["release_date"].asString(),"-")) )
                {
                    fk=1;
                    break;
                }
            }
            if(!fk) continue;            

            if(debug) cout<<" ---- ";
            ra["image"] = replace_string_all(ra["image"].asString(),"150x150.jpg", "500x500.jpg");
            js_cover = js_covers_f+validate_path(erase_rfind(ra["image"].asString(),"/"));
            if(!download_cover(replace_string_all(ra["image"].asString(),"https://","https://i1.wp.com/"),js_cover)) goto for_end;
            img_compare_res = pipe_open(curl_exe+" "+curl_params+" \"http://127.0.0.1:45632/img-compare?paths="+spy_cover_cmd+"|"+js_cover+"\"");

            if(img_compare_res.find(res_success)!=0) continue; 
            fff = stoi(img_compare_res.erase(0, res_success.length()));
            
            spy_cover = spy_covers_f+erase_rfind(scv[fff],"/")+".jpg";
            match_song_in_album(tag,ra,pos,pb,&json_str);
            if(debug) cout<<" ++++ ";
            goto for_end;
        }
        

for_end :

        continue;
    }

    if(pp.execute) pp.terminate();
    if(pb.execute) pb.terminate();
    

    SetCurrentDirectoryA(cd.c_str());
    string out_file = arg_value("--out-file");
    if(out_file != "") out_file = abs_path(validate_path(out_file));


    string failed_file = arg_value("--failed-file");
    if(failed_file != "") failed_file = abs_path(validate_path(failed_file));

    if(out_file=="")     out_file     = pln+"_converted.txt";
    if(failed_file=="")  failed_file  = pln+"_failed.txt";

    out_file     = name_assigner(out_file);
    failed_file  = name_assigner(failed_file);

    ofstream out, failed;

    int ffs=0, fff=0;
    for(size_t i=0; i<tagvs; i++)
    {
        if(conversion[i]!="")
        {
            if(!ffs)
            {
                ffs = 1;
                out.open(out_file);
            }
            else out<<endl;
            out<<tagv[i].url<<"|"<<conversion[i];
        }
        else
        {
            if(!fff)
            {
                fff=1;
                failed.open(failed_file);
            }
            else failed<<endl;
            failed<<tagv[i].url;
        }
    }
    if(ffs) out.close();
    if(fff) failed.close();
    
    string exit_msg;


    if(tagvs==0) s = "";
    else if(found==0) s = "\tConversion failed badly...";
    else if(found==tagvs) s = "\tConversion SUCCESSFUL...";
    else s = "\t"+to_string(found)+" of "+to_string(tagvs)+" songs converted...";
    if(debug) cout<<endl;
    exit_msg = s;
    exiting(exit_msg);
}

