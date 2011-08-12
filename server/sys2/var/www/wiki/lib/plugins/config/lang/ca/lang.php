<?php
/**
 * Catalan language file
 *
 * @author Carles Bellver <carles.bellver@gmail.com>
 * @author carles.bellver@gmail.com
 * @author carles.bellver@cent.uji.es
 * @author Carles Bellver <carles.bellver@cent.uji.es>
 */
$lang['menu']                  = 'Paràmetres de configuració';
$lang['error']                 = 'Els paràmetres no s\'han pogut actualitzar per causa d\'un valor incorrecte Reviseu els canvis i torneu a enviar-los.<br />Els valors incorrectes es ressaltaran amb un marc vermell.';
$lang['updated']               = 'Els paràmetres s\'han actualitzat amb èxit.';
$lang['nochoice']              = '(no hi altres opcions disponibles)';
$lang['locked']                = 'El fitxer de paràmetres no es pot actualitzar. Si això és involuntari, <br />
assegureu-vos que el nom i els permisos del fitxer local de paràmetres són correctes.';
$lang['danger']                = 'Alerta: si canvieu aquesta opció podeu fer que el wiki i el menú de configuració no siguin accessibles.';
$lang['warning']               = 'Avís: modificar aquesta opció pot provocar un comportament no desitjat.';
$lang['security']              = 'Avís de seguretat: modificar aquesta opció pot implicar un risc de seguretat.';
$lang['_configuration_manager'] = 'Gestió de la configuració';
$lang['_header_dokuwiki']      = 'Paràmetres de DokuWiki';
$lang['_header_plugin']        = 'Paràmetres de connectors';
$lang['_header_template']      = 'Paràmetres de plantilles';
$lang['_header_undefined']     = 'Paràmetres no definits';
$lang['_basic']                = 'Paràmetres bàsics';
$lang['_display']              = 'Paràmetres de visualització';
$lang['_authentication']       = 'Paràmetres d\'autenticació';
$lang['_anti_spam']            = 'Paràmetres anti-brossa';
$lang['_editing']              = 'Paràmetres d\'edició';
$lang['_links']                = 'Paràmetres d\'enllaços';
$lang['_media']                = 'Paràmetres de mitjans';
$lang['_advanced']             = 'Paràmetres avançats';
$lang['_network']              = 'Paràmetres de xarxa';
$lang['_plugin_sufix']         = 'Paràmetres de connectors';
$lang['_template_sufix']       = 'Paràmetres de plantilla';
$lang['_msg_setting_undefined'] = 'Falten metadades de paràmetre.';
$lang['_msg_setting_no_class'] = 'Falta classe de paràmetre.';
$lang['_msg_setting_no_default'] = 'No hi ha valor per defecte.';
$lang['fmode']                 = 'Mode de creació de fitxers';
$lang['dmode']                 = 'Mode de creació de directoris';
$lang['lang']                  = 'Idioma';
$lang['basedir']               = 'Directori base';
$lang['baseurl']               = 'URL base';
$lang['savedir']               = 'Directori per desar les dades';
$lang['start']                 = 'Nom de la pàgina d\'inici';
$lang['title']                 = 'Títol del wiki';
$lang['template']              = 'Plantilla';
$lang['license']               = 'Amb quina llicència voleu publicar el contingut?';
$lang['fullpath']              = 'Mostra el camí complet de les pàgines al peu';
$lang['recent']                = 'Canvis recents';
$lang['breadcrumbs']           = 'Nombre d\'engrunes';
$lang['youarehere']            = 'Camí d\'engrunes jeràrquic';
$lang['typography']            = 'Substitucions tipogràfiques';
$lang['htmlok']                = 'Permet HTML incrustat';
$lang['phpok']                 = 'Permet PHP incrustat';
$lang['dformat']               = 'Format de data (vg. la funció PHP <a href="http://www.php.net/strftime">strftime</a>)';
$lang['signature']             = 'Signatura';
$lang['toptoclevel']           = 'Nivell superior per a la taula de continguts';
$lang['tocminheads']           = 'Quantitat mínima d\'encapçalaments que determina si es construeix o no la taula de continguts.';
$lang['maxtoclevel']           = 'Nivell màxim per a la taula de continguts';
$lang['maxseclevel']           = 'Nivell màxim d\'edició de seccions';
$lang['camelcase']             = 'Utilitza CamelCase per als enllaços';
$lang['deaccent']              = 'Noms de pàgina nets';
$lang['useheading']            = 'Utilitza el primer encapçalament per als noms de pàgina';
$lang['refcheck']              = 'Comprova la referència en els fitxers de mitjans';
$lang['refshow']               = 'Nombre de referències de mitjans per mostrar';
$lang['allowdebug']            = 'Permet depuració <strong>inhabiliteu si no és necessari</strong>';
$lang['usewordblock']          = 'Bloca brossa per llista de paraules';
$lang['indexdelay']            = 'Retard abans d\'indexar (segons)';
$lang['relnofollow']           = 'Utilitza rel="nofollow" en enllaços externs';
$lang['mailguard']             = 'Ofusca les adreces de correu';
$lang['iexssprotect']          = 'Comprova codi HTML o Javascript maligne en els fitxers penjats';
$lang['showuseras']            = 'Què cal visualitzar quan es mostra el darrer usuari que ha editat la pàgina';
$lang['useacl']                = 'Utilitza llistes de control d\'accés';
$lang['autopasswd']            = 'Generació automàtica de contrasenyes';
$lang['authtype']              = 'Rerefons d\'autenticació';
$lang['passcrypt']             = 'Mètode d\'encriptació de contrasenyes';
$lang['defaultgroup']          = 'Grup per defecte';
$lang['superuser']             = 'Superusuari: un grup o usuari amb accés complet a totes les pàgines i funcions independentment dels paràmetres ACL';
$lang['manager']               = 'Administrador: un grup o usuari amb accés a certes funcions d\'administració';
$lang['profileconfirm']        = 'Confirma amb contrasenya els canvis en el perfil';
$lang['disableactions']        = 'Inhabilita accions DokuWiki';
$lang['disableactions_check']  = 'Revisa';
$lang['disableactions_subscription'] = 'Subscripció/cancel·lació';
$lang['disableactions_wikicode'] = 'Mostra/exporta font';
$lang['disableactions_other']  = 'Altres accions (separades per comes)';
$lang['sneaky_index']          = 'Per defecte, DokuWiki mostrarà tots els espai en la visualització d\'índex. Si activeu aquest paràmetre, s\'ocultaran aquells espais en els quals l\'usuari no té accés de lectura. Això pot fer que s\'ocultin subespais que sí que són accessibles. En algunes configuracions ACL pot fer que l\'índex resulti inutilitzable.';
$lang['auth_security_timeout'] = 'Temps d\'espera de seguretat en l\'autenticació (segons)';
$lang['securecookie']          = 'Les galetes que s\'han creat via HTTPS, només s\'han d\'enviar des del navegador per HTTPS? Inhabiliteu aquesta opció si només l\'inici de sessió del wiki es fa amb SSL i la navegació del wiki es fa sense seguretat.';
$lang['xmlrpc']                = 'Habilita/inhabilita la interfície XML-RPC';
$lang['xmlrpcuser']            = 'Restringeix l\'accés per XML-RPC als usuaris o grups següents, separats per comes. Deixeu aquest camp en blanc per donar accés a tothom.';
$lang['updatecheck']           = 'Comprova actualitzacions i avisos de seguretat. DokuWiki necessitarà contactar amb update.dokuwiki.org per utilitzar aquesta característica.';
$lang['userewrite']            = 'Utilitza URL nets';
$lang['useslash']              = 'Utilitza la barra / com a separador d\'espais en els URL';
$lang['usedraft']              = 'Desa automàticament un esborrany mentre s\'edita';
$lang['sepchar']               = 'Separador de paraules en els noms de pàgina';
$lang['canonical']             = 'Utilitza URL canònics complets';
$lang['autoplural']            = 'Comprova formes plurals en els enllaços';
$lang['compression']           = 'Mètode de compressió per als fitxers de les golfes';
$lang['cachetime']             = 'Durada màxima de la memòria cau (segons)';
$lang['locktime']              = 'Durada màxima dels fitxers de bloqueig (segons)';
$lang['fetchsize']             = 'Mida màxima (bytes) que fetch.php pot baixar d\'un lloc extern';
$lang['notify']                = 'Envia notificacions de canvis a aquesta adreça de correu';
$lang['registernotify']        = 'Envia informació sobre nous usuaris registrats a aquesta adreça de correu';
$lang['mailfrom']              = 'Adreça de correu remitent per a missatges automàtics';
$lang['gzip_output']           = 'Codifica contingut xhtml com a gzip';
$lang['gdlib']                 = 'Versió GD Lib';
$lang['im_convert']            = 'Camí de la utilitat convert d\'ImageMagick';
$lang['jpg_quality']           = 'Qualitat de compressió JPEG (0-100)';
$lang['subscribers']           = 'Habilita la subscripció a pàgines';
$lang['compress']              = 'Sortida CSS i Javascript compacta';
$lang['hidepages']             = 'Oculta pàgines coincidents (expressions regulars)';
$lang['send404']               = 'Envia "HTTP 404/Page Not Found" per a les pàgines inexistents';
$lang['sitemap']               = 'Genera mapa del lloc en format Google (dies)';
$lang['broken_iua']            = 'No funciona en el vostre sistema la funció ignore_user_abort? Això podria malmetre l\'índex de cerques. Amb IIS+PHP/CGI se sap que no funciona. Vg. <a href="http://bugs.splitbrain.org/?do=details&amp;task_id=852">Bug 852</a> per a més informació.';
$lang['xsendfile']             = 'Utilitza la capçalera X-Sendfile perquè el servidor web distribueixi fitxers estàtics. No funciona amb tots els servidors web.';
$lang['renderer_xhtml']        = 'Renderitzador que cal utilitzar per a la sortida principal (xhtml) del wiki';
$lang['renderer__core']        = '% (ànima del dokuwiki)';
$lang['renderer__plugin']      = '% (connector)';
$lang['rememberme']            = 'Permet galetes de sessió permanents ("recorda\'m")';
$lang['rss_type']              = 'Tipus de canal XML';
$lang['rss_linkto']            = 'Destinació dels enllaços en el canal XML';
$lang['rss_content']           = 'Què es mostrarà en els elements del canal XML?';
$lang['rss_update']            = 'Interval d\'actualització del canal XML (segons)';
$lang['recent_days']           = 'Quantitat de canvis recents que es mantenen (dies)';
$lang['rss_show_summary']      = 'Mostra resum en els títols del canal XML';
$lang['target____wiki']        = 'Finestra de destinació en enllaços interns';
$lang['target____interwiki']   = 'Finestra de destinació en enllaços interwiki';
$lang['target____extern']      = 'Finestra de destinació en enllaços externs';
$lang['target____media']       = 'Finestra de destinació en enllaços de mitjans';
$lang['target____windows']     = 'Finestra de destinació en enllaços de Windows';
$lang['proxy____host']         = 'Nom del servidor intermediari';
$lang['proxy____port']         = 'Port del servidor intermediari';
$lang['proxy____user']         = 'Nom d\'usuari del servidor intermediari';
$lang['proxy____pass']         = 'Contrasenya del servidor intermediari';
$lang['proxy____ssl']          = 'Utilitza SSL per connectar amb el servidor intermediari';
$lang['safemodehack']          = 'Utilitza el hack per a safemode';
$lang['ftp____host']           = 'Servidor FTP per al hack de safemode';
$lang['ftp____port']           = 'Port FTP per al hack de safemode';
$lang['ftp____user']           = 'Nom d\'usuari FTP per al hack de safemode';
$lang['ftp____pass']           = 'Contrasenya FTP per al hack de safemode';
$lang['ftp____root']           = 'Directori arrel FTP per al hack de safemode';
$lang['license_o_']            = 'Cap selecció';
$lang['typography_o_0']        = 'cap';
$lang['typography_o_1']        = 'només cometes dobles';
$lang['typography_o_2']        = 'totes les cometes (podria no funcionar sempre)';
$lang['userewrite_o_0']        = 'cap';
$lang['userewrite_o_1']        = '.htaccess';
$lang['userewrite_o_2']        = 'intern del DokuWiki';
$lang['deaccent_o_0']          = 'desactivat';
$lang['deaccent_o_1']          = 'treure accents';
$lang['deaccent_o_2']          = 'romanització';
$lang['gdlib_o_0']             = 'GD Lib no està disponible';
$lang['gdlib_o_1']             = 'Versió 1.x';
$lang['gdlib_o_2']             = 'Detecció automàtica';
$lang['rss_type_o_rss']        = 'RSS 0.91';
$lang['rss_type_o_rss1']       = 'RSS 1.0';
$lang['rss_type_o_rss2']       = 'RSS 2.0';
$lang['rss_type_o_atom']       = 'Atom 0.3';
$lang['rss_type_o_atom1']      = 'Atom 1.0';
$lang['rss_content_o_abstract'] = 'Resum';
$lang['rss_content_o_diff']    = 'Diff unificat';
$lang['rss_content_o_htmldiff'] = 'Taula de diferències en format HTML';
$lang['rss_content_o_html']    = 'Contingut complet de la pàgina en format HTML';
$lang['rss_linkto_o_diff']     = 'Visualització de diferències';
$lang['rss_linkto_o_page']     = 'pàgina modificada';
$lang['rss_linkto_o_rev']      = 'llista de revisions';
$lang['rss_linkto_o_current']  = 'revisió actual';
$lang['compression_o_0']       = 'cap';
$lang['compression_o_gz']      = 'gzip';
$lang['compression_o_bz2']     = 'bz2';
$lang['xsendfile_o_0']         = 'no utilitzis';
$lang['xsendfile_o_1']         = 'Capçalera pròpia de lighttpd (anterior a la versió 1.5)';
$lang['xsendfile_o_2']         = 'Capçalera X-Sendfile estàndard';
$lang['xsendfile_o_3']         = 'Capçalera X-Accel-Redirect de propietat de Nginx ';
$lang['showuseras_o_loginname'] = 'Nom d\'usuari';
$lang['showuseras_o_username'] = 'Nom complet de l\'usuari';
$lang['showuseras_o_email']    = 'Adreça de correu electrònic de l\'usuari (ofuscada segons el paràmetre de configuració corresponent)';
$lang['showuseras_o_email_link'] = 'Adreça de correu electrònic amb enllaç mailto:';
$lang['useheading_o_0']        = 'Mai';
$lang['useheading_o_navigation'] = 'Només navegació';
$lang['useheading_o_content']  = 'Només contingut wiki';
$lang['useheading_o_1']        = 'Sempre';
