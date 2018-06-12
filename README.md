# squid-redirect

## General info
`squid-redirect` is a tool for managing redirections of [`squid`](http://www.squid-cache.org/) proxy-server. It is easy to use and you can add redirect rule without any effort.

It was tested on Kali Linux.

## How it works
All redirects are stored in one json file called `redirections.json`. You can change this file without restarting `squid `, all changes will be applied immediately.

**Don't forget to leave one empty extra line at the bottom of `redirections.json` file!**

## How to setup
0. You can run `setup.sh` file. It will produce all below operations. Now see 5 step.

1. First of all, we have to install `squid` itself (using apt-get or building by yourself):

```sh
$ sudo apt-get install squid
```

2. Build `squid-redirect` using make:
```sh
make
```

3. The next step is copying `src/main` binary and `redirections.json` file to `/tmp/squid-redirect/` directory (this directory will be changed in the future).

4. After this, copy [squid.conf](./squid.conf) to `/etc/squid/` (make a backup of the original `squid.conf` before replacing it).

5. Restart `squid` and reconfigure it by typing:

```sh
sudo service squid restart
sudo squid -k reconfigure
```

6. Now you can change `/tmp/squid-redirect/redirections.json` file to setup desired redirections.

Also, if you want to **see logs**, then type the next:

```sh
sudo journalctl -u squid.service -b
```
