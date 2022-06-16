#!/usr/bin/php
<!DOCTYPE HTML>
<html>
<head>
    <title>PHP Configuration details</title>
</head>
<body>
    <?php
    // Configs
    ini_set('display_errors', TRUE);
    ?>

    <h1><?php echo "un titre avec PHP"; ?></h1>

    <ul>
        <?php
        foreach (getenv() as $key=>$entry) {
        ?>
            <li><?= $key ?>=<?= $entry ?></li>
        <?php
        }

        var_dump($HTTP_RAW_POST_DATA);
        var_dump($_GET);
        var_dump($_POST);
        var_dump($_FILES);
        ?>
    </ul>

    <?php phpinfo(INFO_ALL); ?>
</body>
</html>