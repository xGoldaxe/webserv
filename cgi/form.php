<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <h1>Data</h1>
    <?php var_dump($_POST) ?>
    <?php var_dump($HTTP_RAW_POST_DATA); ?>
    <hr>
    <h2>Form</h2>
    <form action="." method="post" enctype="multipart/form-data">
        <input type="text" name="test_name" id="testname"> <br>
        <input type="file" name="test_file" id="file"> <br>
        <input type="submit" value="Envoyer">
    </form>
</body>
</html>