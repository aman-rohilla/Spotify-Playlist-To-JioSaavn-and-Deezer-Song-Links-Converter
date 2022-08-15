// Author - Aman-Rohilla@rohilla.co.in

// Compilation Command -
// g++ -o spy2dzr.exe spy2dzr.cpp -std=c++17 -ljsoncpp -lshlwapi -lwsock32 -lwininet -I. -Iinclude -Llib

#include <utils.h> 

size_t found = 0, pos=0, vscs=0, vsca=0;
vector<string> conv_songs, conv_albums, conversion;
string sf = "sources_spy2dzr\\";

string albums_file = sf+"albums.txt";
string songs_file  = sf+"songs.txt";
string token_file  = "spotify_token";
string client_file = "spotify_client";
string curl_exe    = "C:\\Windows\\System32\\curl.exe";
string curl_params = "--silent --max-time 30";
string dzr_dir     = sf+"deezer\\";
string datjf       = dzr_dir+"json\\album_tracks\\";  //deezer album tracks json folder

int match_song_in_album(spy_tag__ &tag, string &str, string album_url, int code=0, string url="")
{
    if(url=="")
    {
        Json::Value ra;
        str2json(str,ra);

        if(!ra["total"].asInt()) return 0;

        int js = ra["data"].size();
        
        for(int i=0; i<js; i++)
            if(ra["data"][i]["isrc"].asString() == tag.isrc)
            {
                conversion[pos] = ra["data"][i]["link"].asString();
                url = conversion[pos];
                break;
            }

        if(code) write_to_text(datjf+erase_rfind(album_url,"/")+".json",str,0);
        
        if(code==2) 
        {
            write_to_text(datjf+erase_rfind(album_url,"/")+".json",str,0);
            write_to_text(albums_file,tag.album_url+"|"+album_url);
            conv_albums.push_back(tag.album_url+"|"+album_url);
            psv_sorter_vec(conv_albums,1);
            vsca++;
        }
    }
    else conversion[pos] = url;

    write_to_text(songs_file,tag.url+"|"+url);
    conv_songs.push_back(tag.url+"|"+url);
    vscs++;
    psv_sorter_vec(conv_songs,1);
    
    found++;
    return 1;
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
      Spotify Playlist To Deezer Song Links Converter By Aman-Rohilla@rohilla.co.in
_________________________________________________________________________________________

      Usage : spy2dzr.exe spotify-playlist-link
      Flags : out-file, failed-file, token, client, no-saving-token-client
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
      Spotify Playlist To Deezer Song Links Converter By Aman-Rohilla@rohilla.co.in
_________________________________________________________________________________________

)";

    set_console_title("Spotify -> Deezer");
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
        save_token_client = !arg_find("--no-saving-token-client");
    }

    progress_bar_param pb;
    pb.task = "\tProcessing : ";
    static pause_param pp(&pb);
    pp.title="Spotify To Deezer";

    if(!network_connection()) exiting("\tNETWORK DISCONNECTED...");
    atexit([](){console_cursor(1);});
    atexit([](){setConsoleColor(7);});
    atexit([](){pp.terminate();});

    string cd = directory()+"\\";
    string sd = ext_dn(exe_path());
    SetCurrentDirectoryA(sd.c_str());

    md_if(datjf);
    md_if(sf);

    if(!exists(datjf)) exiting("\t"+ext_dn(datjf)+" directory is not writable...");

    size_t f;

    vector<spy_tag__> tagv;
    string s, t, pln;

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

    ////////// Setting Variables

    // pp.delayed_message=1;

    text_to_vector(songs_file,conv_songs);
    text_to_vector(albums_file,conv_albums);


    psv_sorter_vec(conv_songs,1);
    psv_sorter_vec(conv_albums,1);

    vscs=conv_songs.size();
    vsca=conv_albums.size();

    if(pp.execute) pp.launch();
    if(pb.execute) pb.total_count = tagvs;

    string json_str;
    string spy_album;
    string cover;

    int js=0;
    pb.set_len = 37;

    spy_tag__ tag;

    conversion.resize(tagvs);
    for(pos=0; pos<tagvs; pos++)
    {
        vector<string> match, links, album_links;
        Json::Value r, rs, ras, ris;
        pp.resume();

        if(pb.execute)
        {
            pb.current_count++;
            if(pos) pb.extra_info = "Found : "+to_string(found);
            pb.new_line = "\t" + tagv[pos].title+" - "+tagv[pos].album;
            pb.update();
        }
        
        tag = tagv[pos];
        string upc;

        f = binary_search_string_vec(conv_songs,0,vscs,tag.url,1);
        if(f!=-1)
        {   
            conversion[pos] = get_column(conv_songs[f],2);
            found++;
            goto for_end;
        }

        f = binary_search_string_vec(conv_albums,0,vsca,tag.album_url,1);
        if(f!=-1)
        {
            json_str = text_to_string(datjf+erase_rfind(get_column(conv_albums[f],2),"/")+".json");
            if(match_song_in_album(tagv[pos], json_str, get_column(conv_albums[f],2), 1)) goto for_end;
        }


        spy_album = tag.album;
        try {
            ras = spy_get_album_json(tag.album_url,token);
        } catch(string error) {
            if(error=="AccessTokenExpired") {
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
        }
        if(!ras.isMember("external_ids")) goto for_end;

        upc = ras["external_ids"]["upc"].asString();
        while(upc[0]=='0') upc.erase(0,1);

        json_str = pipe_open(curl_exe+" "+curl_params+" https://api.deezer.com/album/upc:"+upc);
        str2json(json_str,rs);

        if(rs.isMember("id"))
        {
            json_str = pipe_open(curl_exe+" "+curl_params+" "+rs["tracklist"].asString());
            if(match_song_in_album(tagv[pos], json_str, rs["link"].asString(),2)) goto for_end;
        }

        if(valid_album_title(tag.album))
        {
            json_str = pipe_open(curl_exe+" "+curl_params+" -X \"GET\" \"https://api.spotify.com/v1/search?q=isrc%3A"+tag.isrc+"&type=track&market=IN&limit=50\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer "+token+"\"");   
            str2json(json_str,ris);
            if(ris["error"]["message"]=="The access token expired") exiting("\tAccess token got expired while processing...");
            if(ris.isMember("tracks"))
            {
                js = ris["tracks"]["items"].size();
                string album;
                string spy_album = extract_album(tag.album);

                for(int i=0; i<js; i++)
                {
                    album = ris["tracks"]["items"][i]["album"]["name"].asString();

                    if(!valid_album_title(album) && copy_str(album,0,spy_album.length()-1)==spy_album)
                    {
                        try {
                            ras = spy_get_album_json(ris["tracks"]["items"][i]["album"]["uri"].asString(),token);
                        } catch (string error) {
                            if(error=="AccessTokenExpired") {
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

                            }                         }
                        if(!ras.isMember("external_ids")) goto for_end;

                        upc = ras["external_ids"]["upc"].asString();
                        while(upc[0]=='0') upc.erase(0,1);

                        json_str = pipe_open(curl_exe+" "+curl_params+" https://api.deezer.com/album/upc:"+upc);
                        str2json(json_str,rs);
                        if(rs.isMember("id"))
                        {
                            json_str = pipe_open(curl_exe+" "+curl_params+" "+rs["tracklist"].asString());
                            if(match_song_in_album(tagv[pos], json_str, rs["link"].asString()),1) goto for_end;
                        }
                    }
                }
            }
        }
        

        json_str = pipe_open(curl_exe+" "+curl_params+" https://api.deezer.com/track/isrc:"+tag.isrc);
        str2json(json_str,ris);
        
        if(ris.isMember("link")) match_song_in_album(tagv[pos], json_str, "", 0, ris["link"].asString());
        
        
for_end :

        if(pb.execute && pos==tagvs-1)
        {
            pb.extra_info = "Found : "+to_string(found);
            pb.update(1);
        }
    }

    if(pp.execute) pp.terminate();
    if(pb.execute) pb.terminate();
    
    pln = validate_path(pln);

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
            if(ffs)
                out<<endl;
            else {
                ffs=1;
                out.open(out_file);
            }

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


    if(!tagvs)              s = "\tNOTHING TO CONVERT...";
    else if(found==tagvs)   s = "\tCONVERSION SUCCESSFUL...";
    else if(found==0)       s = "\tConversion failed badly...";
    else                    s = "\t"+to_string(found)+" of "+to_string(tagvs)+" Songs Converted...";

    exiting(s);
}

