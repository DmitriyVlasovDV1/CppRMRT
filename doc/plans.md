<h1>Планы на будущее:</h1>
_______________________________________
<h3>Render (Часть Святослава):</h3>
<ul>
  <li>До MVP (~10 апреля):</li>
    <ul>
      <li>Закончить делать класс топологии:</li>
        <ul>
            <li>
                Этот класс позволяет пользователю (програмисту юнитов)
                создавать шаблонные примитивы, такие как: сфера, куб и тп.<br>
                Это нужно для дебажного вывода и второго вида отрисовки ray marching-a.
            </li>
        </ul>
      <li>Доделать загрузку моделей в формате .obj;</li>
      <li>Добавить освещение без типизации самих источников света:</li>
        <ul>
            <li>
                Подразумевается супер простая реализация направленного источника света
                на шейдере без создания одноименного класса в структуре рендера.
            </li>
        </ul>
    </ul>
Также планируется добавление юнитов с демонстрацией загрузки моделей и вывода шаблонных примитивов.
  <li>После MVP:</li>
    <ul>
      <li>Класс текстур и FBO (frame buffer object);</li>
      <li>
        Освещение и поддержка множества источников освещения: это касается как их
        количества на сцене так и количества их видов (точечный, направленный, прожекторный).
      </li>
    </ul>
</ul>

<h3>Universal scene utility (Часть Дмитрия):</h3>
<ul>
  <li>После MVP:</li>
    <ul>
      <li>Сохраниение и загрузка универсальной сцены из файла</li>
      <li>2d-редактор сцены</li>
    </ul>
</ul>

<h3>RM render (Часть Дмитрия):</h3>
<ul>
  <li>После MVP:</li>
    <ul>
      <li>Добавить текстуры</li>
      <li>Добавить преломление</li>
    </ul>
</ul>
