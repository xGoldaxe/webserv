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
        foreach (getenv() as $key=>entry) {
        ?>
            <li><?= $key ?>=<?= $entry ?></li>
        <?php
        }
        ?>
    </ul>

    <?php phpinfo(INFO_ALL); ?>
</body>
</html>