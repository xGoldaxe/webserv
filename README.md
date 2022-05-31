# Project: webserv
for 42 paris

### Tests

Pour utiliser les tests:
 1. Déplacez vous dans l'un des dossiers contenu dans le dossier `tests`.
 2. Démarrez ensuite le serveur dans un terminal `./server.sh`
 3. Dans un autre terminal, lancez le client `./client.sh`
 4. (*optionnel*) Uniquement les logs du client sont sauvegardées (cf. Dossier `logs/`).

/!\ Pensez à couper votre serveur après des tests pour éviter les collisions /!\

### Syntaxe Fichier Config

server
{
	error_page 404 403 = error.html ;
	body_max_size = 2048 ;
	...
	location /
	{
		root ./ ;
		index = index.html index.htm ;
		error_page 404 = 404.html ;
		methods = POST GET DELETE ;
		...
	}
	...
}