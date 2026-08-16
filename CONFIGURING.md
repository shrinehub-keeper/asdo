# Configuring asdo

`asdo` is [OpenDoas](https://github.com/Duncaen/OpenDoas) with a few small
additions on top: insults on failed authentication, and a customizable
password prompt / error message. Every existing `doas.conf` rule and option
(`permit`, `deny`, `nopass`, `persist`, `keepenv`, `setenv`, ...) works
exactly as it does upstream — see `doas.conf(5)` for those. This document
only covers what `asdo` adds.

*This document is Copyright (c) 2026 asdo contributors, licensed under
the GNU General Public License v2 (or later) — see `LICENSE.GPLv2`.
The underlying OpenDoas project it documents remains ISC-licensed —
see `LICENSE`.*

## Config file location

`asdo` reads its configuration from `/etc/asdo.conf`. If that file does not
exist, it falls back to `/etc/doas.conf`, so an existing OpenDoas/doas setup
keeps working untouched after switching to `asdo`.

The `-C <config>` flag still lets you point at an arbitrary file for
`asdo -C <config> <command>` config-check invocations, exactly as in
upstream doas, and is not affected by the fallback above.

## New directives

These are global directives (not rule options), and may appear anywhere in
the config file, one per line:

```
insults on
prompt "asdo (%u@%h) password: "
prompt_err "Sorry, wrong password."
```

### `insults on|off`

Default: `off`.

When enabled, a random insult is printed to stderr instead of the normal
`Authentication failed` message whenever authentication fails (wrong
password, locked/invalid account, etc). Like doas/OpenDoas, `asdo` only ever
makes a single authentication attempt per invocation — there is no retry
loop, so at most one insult is printed per run, and the process still exits
non-zero exactly as OpenDoas does on an auth failure.

The insult pool includes every insult shipped with upstream `sudo`'s
`--with-insults` build option (the "classic", "csops", "goons", "2001" and
"python" sets), plus a handful of `asdo`-specific ones.

### `prompt "text"`

Overrides the password prompt text. Two placeholders are expanded:

| Placeholder | Expands to               |
|-------------|---------------------------|
| `%u`        | the invoking user's name |
| `%h`        | the local hostname        |
| `%%`        | a literal `%`             |

If omitted, the prompt defaults to the standard doas prompt,
`doas (%u@%h) password: `.

### `prompt_err "text"`

Overrides the message printed on authentication failure when `insults` is
`off` (or hasn't fired). If omitted, the standard `Authentication failed`
message is used. This directive has no effect when `insults on` is set —
insults take priority.

## Environment variable overrides

Two environment variables let a user override the prompt/error text for
their own invocations without editing the config file. They take priority
over the config directives above:

- `ASDO_PS1` — overrides the password prompt (same `%u` / `%h` / `%%`
  placeholders as `prompt`).
- `ASDO_ERR` — overrides the authentication failure message (same as
  `prompt_err`; still overridden by `insults on`).

```
$ ASDO_PS1='gimme the secret, %u: ' asdo whoami
```

Both variables are only ever used to build text that is shown back to the
invoking user (the prompt and the error message) — they are never
interpreted as a `printf`-style format string, so they cannot be used to
read or corrupt process memory even though `asdo` runs setuid root.

## Example

```
# /etc/asdo.conf
insults on
prompt "asdo (%u@%h) password: "

permit persist :wheel
permit nopass root
```

## Reserved words

`insults`, `prompt` and `prompt_err` are now reserved keywords at the start
of a config line, alongside the existing `permit`/`deny`/`as`/`cmd`/`args`/
`nopass`/`nolog`/`persist`/`keepenv`/`setenv`. A username or group that
happens to collide with one of these words needs to be expressed
differently (e.g. via its numeric uid/gid) — the same limitation that
already applies to the original keywords in upstream doas.
