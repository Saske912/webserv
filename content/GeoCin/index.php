<?php

//подключаем шаблонизатор
include ('basic.php');

//здесь хранится контент
$pages_dir='pages/';

$select='active';

// echo "_SERVER: \n";
// var_dump ($_SERVER);
// echo "_GET: \n";
// foreach ($_GET as &$k) {
//     echo $k;
//     echo "\n";
// }

if(!isset($_GET['z']))
    $content=file_get_contents($pages_dir.'index.html');
else{
    $page=$_GET['z'];
    $file=$pages_dir.$page.'.html';

    if(file_exists($file)) {
        $content=file_get_contents($file);
    } else {
        header("HTTP/1.1 404 Not Found");
    }
}

//загружаем шаблон
$tpl->get_tpl('basic.html');


//устанавливаем переменные
$tpl->set_value('HOME',"главная страница");
$tpl->set_value('ABOUT',"о нас");
$tpl->set_value('CLASSES',"асортимент");
$tpl->set_value('BLOG',"блог");
$tpl->set_value('CONTACT',"связаться");
$tpl->set_value('CONTENT',$content);

//определяем загруженный контент и подсвечиваем выделенную директорию
switch ($page) {
    case 'about':
        $tpl->set_value('JS', "js/about.js");
        $tpl->set_value('CSS1', "about");
        $tpl->set_value('CSS2', "about_");
        $tpl->set_value('TITLE', "GeoCin - о нас");
        $tpl->set_value('class2', $select);
        break;
    case 'classes':
        $tpl->set_value('JS', "js/classes.js");
        $tpl->set_value('CSS1', "classes");
        $tpl->set_value('CSS2', "classes_");
        $tpl->set_value('TITLE', "GeoCin - асортимент");
        $tpl->set_value('class3', $select);
        break;
    case 'blog':
        $tpl->set_value('JS', "js/blog.js");
        $tpl->set_value('CSS1', "blog");
        $tpl->set_value('CSS2', "blog_");
        $tpl->set_value('TITLE', "GeoCin - блог");
        $tpl->set_value('class4', $select);
        break;
    case 'contact':
        $tpl->set_value('GOOGLE', "https://maps.googleapis.com/maps/api/js?v=3.exp&key=AIzaSyCIwF204lFZg1y4kPSIhKaHEXMLYxxuMhA");
        $tpl->set_value('JS', "js/contact.js");
        $tpl->set_value('CSS1', "contact");
        $tpl->set_value('CSS2', "contact_");
        $tpl->set_value('TITLE', "GeoCin - связаться");
        $tpl->set_value('class5', $select);
        break;
    default:
        $tpl->set_value('JS', "js/custom.js");
        $tpl->set_value('CSS1', "main_styles");
        $tpl->set_value('TITLE', "GeoCin - главная страница");
        $tpl->set_value('class1', $select);
        break;
}


//выполняем парсинг шаблона
$tpl->tpl_parse();

//выводим полученный HTML-код
echo $tpl->html;
