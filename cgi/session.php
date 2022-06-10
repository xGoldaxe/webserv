
<?php
session_start();
if (!isset($_SESSION['count'])) {
  $_SESSION['count'] = 0;
} else {
  $_SESSION['count']++;
}

echo "Nombre de fois que la page a été rafraichie: {$_SESSION['count']}";
?>
