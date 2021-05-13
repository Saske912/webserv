<?php
//класс шаблона
class MetaDataBase
{
    public $values=[];//переменные шаблона
    public $html;//HTML-код

    //функция загрузки шаблона
    function get_tpl($tpl_name)
    {
        if(empty($tpl_name)||!file_exists($tpl_name))
        {
            return false;
        }
        else
        {
            $this->html=implode('',file($tpl_name));
        }
    }

    // Функция принимает ассоциативный масси и создает несколько
        // тегов <option value "$key">$value, где $key - очередной
        // ключ массива, а $value - очередное значение. Если задан
        // также и второй параметр, то у соответствующего тега option
        // проставляется атрибут selected.
        function selectItems($items, $selected = 0)
        {
            $text = "";
            foreach ($items as $k => $v) {
            if ($k === $selected)
                $ch = " selected";
            else
                $ch = "";
            $text .= "<option$ch value='$k'>$v</option>\n";
            }
        return $text;
        }

    //функция установки значения
    function set_value($key,$var)
    {
        $key='{'.$key.'}';
        $this->values[$key]=$var;
    }

    //парсинг шаблона
    function tpl_parse()
    {
        foreach ($this->values as $find=>$replace)
        {
            $this->html=str_replace($find,$replace,$this->html);
        }
    }
}

//экземпляр класса
$tpl=new MetaDataBase;