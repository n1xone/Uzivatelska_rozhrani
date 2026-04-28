import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from datetime import datetime
import copy, csv

BG       = "#f0f2f5"
WHITE    = "#ffffff"
SIDEBAR  = "#ffffff"
BLUE     = "#2563eb"
BLUE_LT  = "#eff6ff"
RED      = "#ef4444"
RED_LT   = "#fef2f2"
GREEN    = "#16a34a"
GREEN_LT = "#f0fdf4"
GRAY_50  = "#f9fafb"
GRAY_100 = "#f3f4f6"
GRAY_200 = "#e5e7eb"
GRAY_400 = "#9ca3af"
GRAY_600 = "#4b5563"
GRAY_900 = "#111827"
ORANGE   = "#f59e0b"
PURPLE   = "#8b5cf6"
FM       = "Segoe UI"

CAT_COLORS = {
    "Jidlo":    ("#fee2e2", "#ef4444"),
    "Bydleni":  ("#dbeafe", "#3b82f6"),
    "Doprava":  ("#fef3c7", "#f59e0b"),
    "Sport":    ("#dcfce7", "#22c55e"),
    "Obleceni": ("#fae8ff", "#a855f7"),
    "Zabava":   ("#ffedd5", "#f97316"),
    "Vzdelani": ("#e0f2fe", "#0ea5e9"),
    "Ostatni":  ("#f3f4f6", "#6b7280"),
}
CAT_LABELS = {
    "Jidlo": "Jidlo", "Bydleni": "Bydleni", "Doprava": "Doprava",
    "Sport": "Sport", "Obleceni": "Obleceni", "Zabava": "Zabava",
    "Vzdelani": "Vzdelani", "Ostatni": "Ostatni",
}

CAT_DISPLAY = {
    "Jidlo": "Jidlo", "Bydleni": "Bydleni", "Doprava": "Doprava",
    "Sport": "Sport", "Obleceni": "Obleceni", "Zabava": "Zabava",
    "Vzdelani": "Vzdelani", "Ostatni": "Ostatni",
}

CATS = list(CAT_COLORS.keys())

BANK_DATA = {
    "CSOB": [
        {"date": "20. 2. 2026", "desc": "Nakup v Albert",      "cat": "Jidlo",    "amount": -350},
        {"date": "20. 2. 2026", "desc": "Obed v kavarne",      "cat": "Jidlo",    "amount": -150},
        {"date": "1. 2. 2026",  "desc": "Najem za byt",        "cat": "Bydleni",  "amount": -8500},
        {"date": "1. 2. 2026",  "desc": "Mesicni jizdenka",    "cat": "Doprava",  "amount": -550},
        {"date": "5. 2. 2026",  "desc": "Posilovna - mesicni", "cat": "Sport",    "amount": -280},
        {"date": "1. 2. 2026",  "desc": "Stipendium",          "cat": "Vzdelani", "amount": 12000},
        {"date": "3. 2. 2026",  "desc": "Prace na dohodu",     "cat": "Ostatni",  "amount": 5000},
        {"date": "7. 2. 2026",  "desc": "Tesco - potraviny",   "cat": "Jidlo",    "amount": -420},
        {"date": "10. 2. 2026", "desc": "Kino",                "cat": "Zabava",   "amount": -180},
        {"date": "15. 2. 2026", "desc": "Spotify Premium",     "cat": "Zabava",   "amount": -159},
    ],
    "Komercni banka": [
        {"date": "2. 2. 2026",  "desc": "Prevod - sporeni",    "cat": "Ostatni",  "amount": 3000},
        {"date": "5. 2. 2026",  "desc": "Lidl nakup",          "cat": "Jidlo",    "amount": -280},
        {"date": "8. 2. 2026",  "desc": "Netflix",             "cat": "Zabava",   "amount": -239},
        {"date": "10. 2. 2026", "desc": "Benzin",              "cat": "Doprava",  "amount": -1200},
        {"date": "14. 2. 2026", "desc": "Plat",                "cat": "Ostatni",  "amount": 22000},
        {"date": "17. 2. 2026", "desc": "Restaurace",          "cat": "Jidlo",    "amount": -650},
        {"date": "20. 2. 2026", "desc": "Elektrina",           "cat": "Bydleni",  "amount": -1800},
    ],
    "Ceska sporitelna": [
        {"date": "1. 2. 2026",  "desc": "Najemne",             "cat": "Bydleni",  "amount": -9500},
        {"date": "3. 2. 2026",  "desc": "Plat",                "cat": "Ostatni",  "amount": 18000},
        {"date": "6. 2. 2026",  "desc": "Kaufland",            "cat": "Jidlo",    "amount": -520},
        {"date": "9. 2. 2026",  "desc": "MHD rocni",           "cat": "Doprava",  "amount": -310},
        {"date": "11. 2. 2026", "desc": "Obleceni Zara",       "cat": "Obleceni", "amount": -1490},
        {"date": "16. 2. 2026", "desc": "Knizky",              "cat": "Vzdelani", "amount": -450},
    ],
    "Moneta Money Bank": [
        {"date": "2. 2. 2026",  "desc": "Vyplata",             "cat": "Ostatni",  "amount": 15000},
        {"date": "4. 2. 2026",  "desc": "Albert velky nakup",  "cat": "Jidlo",    "amount": -890},
        {"date": "7. 2. 2026",  "desc": "Internet",            "cat": "Ostatni",  "amount": -499},
        {"date": "12. 2. 2026", "desc": "Gym",                 "cat": "Sport",    "amount": -350},
        {"date": "18. 2. 2026", "desc": "Vecere s prateli",    "cat": "Jidlo",    "amount": -480},
    ],
    "Revolut": [
        {"date": "5. 2. 2026",  "desc": "Amazon.de",           "cat": "Ostatni",  "amount": -1250},
        {"date": "8. 2. 2026",  "desc": "Uber Eats",           "cat": "Jidlo",    "amount": -320},
        {"date": "10. 2. 2026", "desc": "Airbnb platba",       "cat": "Bydleni",  "amount": -3200},
        {"date": "13. 2. 2026", "desc": "Prevod od rodice",    "cat": "Ostatni",  "amount": 5000},
        {"date": "19. 2. 2026", "desc": "Bolt",                "cat": "Doprava",  "amount": -180},
        {"date": "21. 2. 2026", "desc": "Apple App Store",     "cat": "Zabava",   "amount": -99},
    ],
    "Wise": [
        {"date": "1. 2. 2026",  "desc": "Prijem z freelance",  "cat": "Ostatni",  "amount": 8500},
        {"date": "6. 2. 2026",  "desc": "Hostinger hosting",   "cat": "Vzdelani", "amount": -399},
        {"date": "11. 2. 2026", "desc": "GitHub Copilot",      "cat": "Vzdelani", "amount": -250},
        {"date": "15. 2. 2026", "desc": "Figma Pro",           "cat": "Vzdelani", "amount": -450},
        {"date": "20. 2. 2026", "desc": "Dribbble",            "cat": "Zabava",   "amount": -350},
    ],
}

BANKS = list(BANK_DATA.keys())


class SmartBudgetApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Chytry rozpocet / Smart Budget")
        self.geometry("1280x780")
        self.minsize(1100, 680)
        self.configure(bg=BG)
        self.resizable(True, True)

        self.bank_transactions = {b: copy.deepcopy(v) for b, v in BANK_DATA.items()}
        self.active_page  = "overview"
        self.search_var   = tk.StringVar()
        self.search_var.trace_add("write", self._on_search)
        self.cat_filters  = {c: tk.BooleanVar(value=False) for c in CATS}
        self.current_bank = tk.StringVar(value="CSOB")
        self.tree = None

        self._build_ui()
        self._set_nav_active("overview")
        self._show_overview()

    @property
    def transactions(self):
        return self.bank_transactions[self.current_bank.get()]

    def _build_ui(self):
        self._build_topbar()
        tk.Frame(self, bg=GRAY_200, height=1).pack(fill=tk.X)
        body = tk.Frame(self, bg=BG)
        body.pack(fill=tk.BOTH, expand=True)
        self._build_sidebar(body)
        self.content = tk.Frame(body, bg=BG)
        self.content.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=16, pady=16)

    def _build_topbar(self):
        bar = tk.Frame(self, bg=WHITE, height=56)
        bar.pack(fill=tk.X)
        bar.pack_propagate(False)

        lf = tk.Frame(bar, bg=WHITE)
        lf.pack(side=tk.LEFT, padx=20, pady=8)
        tk.Label(lf, text="📋", font=(FM, 16), bg=WHITE, fg=BLUE).pack(side=tk.LEFT)
        tk.Label(lf, text=" Chytry rozpocet", font=(FM, 14, "bold"), fg=GRAY_900, bg=WHITE).pack(side=tk.LEFT)
        tk.Label(lf, text=" / Smart Budget",  font=(FM, 12), fg=GRAY_400, bg=WHITE).pack(side=tk.LEFT)

        rf = tk.Frame(bar, bg=WHITE)
        rf.pack(side=tk.RIGHT, padx=16)

        tk.Button(rf, text="⇥  Odhlasit", font=(FM, 10), fg=RED, bg=WHITE,
                  bd=0, cursor="hand2", activebackground=WHITE, activeforeground="#b91c1c",
                  command=self._do_odhlasit).pack(side=tk.RIGHT, padx=8)

        tk.Button(rf, text="Nastaveni", font=(FM, 10), fg=GRAY_600, bg=WHITE,
                  bd=0, cursor="hand2", relief=tk.FLAT,
                  activebackground=GRAY_100, activeforeground=GRAY_900,
                  command=lambda: self._navigate("settings")).pack(side=tk.RIGHT, padx=8)

        tk.Button(rf, text="Soubor", font=(FM, 10), fg=GRAY_600, bg=WHITE,
                  bd=0, cursor="hand2", relief=tk.FLAT,
                  activebackground=GRAY_100, activeforeground=GRAY_900,
                  command=self._do_soubor).pack(side=tk.RIGHT, padx=8)

        tk.Label(rf, text="👤  Nikita Chervinskyi", font=(FM, 10),
                 fg=GRAY_600, bg=WHITE).pack(side=tk.RIGHT, padx=14)

        bf = tk.Frame(rf, bg=GRAY_100, padx=10, pady=5)
        bf.pack(side=tk.RIGHT, padx=12)
        tk.Label(bf, text="🏦", bg=GRAY_100, fg=GRAY_600, font=(FM, 11)).pack(side=tk.LEFT)
        cb = ttk.Combobox(bf, textvariable=self.current_bank, values=BANKS,
                          width=17, font=(FM, 10), state="readonly")
        cb.pack(side=tk.LEFT)
        self.current_bank.trace_add("write", self._on_bank_change)

    def _do_odhlasit(self):
        if messagebox.askyesno("Odhlasit",
                               "Opravdu se chcete odhlasit?\nAplikace bude ukoncena.",
                               icon="warning"):
            self.destroy()

    def _do_soubor(self):
        win = tk.Toplevel(self)
        win.title("Soubor")
        win.geometry("300x170")
        win.resizable(False, False)
        win.configure(bg=WHITE)
        win.grab_set()
        win.transient(self)

        tk.Label(win, text="Soubor / File", font=(FM, 12, "bold"),
                 fg=GRAY_900, bg=WHITE).pack(pady=(16, 10))
        tk.Button(win, text="📤  Exportovat transakce (CSV)", font=(FM, 10),
                  fg=WHITE, bg=BLUE, bd=0, padx=12, pady=8, cursor="hand2",
                  activebackground="#1d4ed8",
                  command=lambda: [self._export_csv(), win.destroy()]).pack(fill=tk.X, padx=28, pady=3)
        tk.Button(win, text="Zavrit", font=(FM, 10), fg=GRAY_600, bg=GRAY_100,
                  bd=0, padx=12, pady=8, cursor="hand2",
                  activebackground=GRAY_200,
                  command=win.destroy).pack(fill=tk.X, padx=28, pady=3)

    def _export_csv(self):
        bank = self.current_bank.get()
        path = filedialog.asksaveasfilename(
            defaultextension=".csv",
            filetypes=[("CSV soubor", "*.csv")],
            initialfile=f"transakce_{bank.replace(' ', '_')}.csv",
            title="Ulozit transakce jako CSV"
        )
        if not path:
            return
        with open(path, "w", newline="", encoding="utf-8-sig") as f:
            writer = csv.DictWriter(f, fieldnames=["date", "desc", "cat", "amount"])
            writer.writeheader()
            writer.writerows(self.transactions)
        messagebox.showinfo("Export hotov", f"Transakce ulozeny:\n{path}")

    def _on_bank_change(self, *_):
        if hasattr(self, "content"):
            self.tree = None
            self._update_remaining_card()
            self._refresh_page()

    def _build_sidebar(self, parent):
        sb = tk.Frame(parent, bg=SIDEBAR, width=220)
        sb.pack(side=tk.LEFT, fill=tk.Y)
        sb.pack_propagate(False)
        tk.Frame(sb, bg=GRAY_200, width=1).pack(side=tk.RIGHT, fill=tk.Y)

        nav_items = [
            ("overview",  "📊", "Prehled",        "Overview"),
            ("add",       "➕", "Pridat polozku",  "Add Item"),
            ("stats",     "📈", "Statistiky",      "Statistics"),
            ("goals",     "🎯", "Cile",            "Goals"),
        ]
        self.nav_btns = {}
        tk.Frame(sb, height=12, bg=SIDEBAR).pack()
        for key, icon, cz, en in nav_items:
            self.nav_btns[key] = self._nav_btn(sb, key, icon, cz, en)

        card = tk.Frame(sb, bg=BLUE_LT, padx=14, pady=12)
        card.pack(side=tk.BOTTOM, fill=tk.X, padx=12, pady=16)
        tk.Label(card, text="Zbyvá do konce mesice:", font=(FM, 9),
                 fg=GRAY_600, bg=BLUE_LT).pack(anchor=tk.W)
        self.remaining_lbl = tk.Label(card, text="", font=(FM, 20, "bold"), fg=BLUE, bg=BLUE_LT)
        self.remaining_lbl.pack(anchor=tk.W)
        self.remaining_sub = tk.Label(card, text="", font=(FM, 9), fg=GRAY_400, bg=BLUE_LT)
        self.remaining_sub.pack(anchor=tk.W)
        self._update_remaining_card()

    def _update_remaining_card(self):
        inc = sum(t["amount"] for t in self.transactions if t["amount"] > 0)
        exp = sum(t["amount"] for t in self.transactions if t["amount"] < 0)
        bal = inc + exp
        self.remaining_lbl.configure(text=f"{bal:,} Kc".replace(",", " "))
        per = bal // 8 if bal > 0 else 0
        self.remaining_sub.configure(text=f"Na den: ~{per:,} Kc".replace(",", " "))

    def _nav_btn(self, parent, key, icon, cz, en):
        f = tk.Frame(parent, bg=SIDEBAR, cursor="hand2")
        f.pack(fill=tk.X, padx=8, pady=2)
        sub = tk.Frame(f, bg=SIDEBAR, padx=10, pady=8)
        sub.pack(fill=tk.X)
        icon_lbl = tk.Label(sub, text=icon, font=(FM, 11), bg=SIDEBAR, fg=GRAY_600)
        icon_lbl.pack(side=tk.LEFT)
        rf2 = tk.Frame(sub, bg=SIDEBAR)
        rf2.pack(side=tk.LEFT, padx=8)
        title = tk.Label(rf2, text=cz, font=(FM, 11, "bold"), fg=GRAY_900, bg=SIDEBAR, anchor=tk.W)
        title.pack(anchor=tk.W)
        sub2  = tk.Label(rf2, text=en, font=(FM, 9), fg=GRAY_400, bg=SIDEBAR, anchor=tk.W)
        sub2.pack(anchor=tk.W)

        all_w = (f, sub, icon_lbl, rf2, title, sub2)

        def enter(e):
            if self.active_page != key:
                for w in all_w: w.configure(bg=GRAY_100)
        def leave(e):
            col = BLUE_LT if self.active_page == key else SIDEBAR
            for w in all_w: w.configure(bg=col)

        for w in all_w:
            w.bind("<Button-1>", lambda e, k=key: self._navigate(k))
            w.bind("<Enter>", enter)
            w.bind("<Leave>", leave)

        return {"frame": f, "sub": sub, "icon": icon_lbl, "rf2": rf2, "title": title, "sub2": sub2}

    def _set_nav_active(self, key):
        for k, widgets in self.nav_btns.items():
            col = BLUE_LT if k == key else SIDEBAR
            for w in widgets.values():
                try: w.configure(bg=col)
                except: pass
            widgets["title"].configure(fg=BLUE if k == key else GRAY_900)

    def _navigate(self, key):
        self.active_page = key
        self.tree = None
        self._set_nav_active(key)
        self._refresh_page()

    def _refresh_page(self):
        for w in self.content.winfo_children():
            w.destroy()
        if   self.active_page == "overview":  self._show_overview()
        elif self.active_page == "add":       self._show_add_item()
        elif self.active_page == "stats":     self._show_stats()
        elif self.active_page == "goals":     self._show_goals()
        elif self.active_page == "settings":  self._show_settings()

    def _show_overview(self):
        self._update_remaining_card()
        c    = self.content
        txns = self.transactions
        inc  = sum(t["amount"] for t in txns if t["amount"] > 0)
        exp  = sum(t["amount"] for t in txns if t["amount"] < 0)
        bal  = inc + exp

        cf = tk.Frame(c, bg=BG)
        cf.pack(fill=tk.X, pady=(0, 14))
        self._sum_card(cf, "Prijmy / Income",   f"+{inc:,} Kc".replace(",", " "), GREEN, "📈")
        self._sum_card(cf, "Vydaje / Expenses",  f"{exp:,} Kc".replace(",", " "), RED,  "📉")
        bs = (f"+{bal:,} Kc" if bal >= 0 else f"{bal:,} Kc").replace(",", " ")
        self._sum_card(cf, "Bilance / Balance",  bs, BLUE, "🏦")

        row = tk.Frame(c, bg=BG)
        row.pack(fill=tk.BOTH, expand=True)
        left = tk.Frame(row, bg=WHITE)
        left.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 12))
        self._border(left)
        self._build_txn_list(left)

        right = tk.Frame(row, bg=WHITE, width=280)
        right.pack(side=tk.RIGHT, fill=tk.Y)
        right.pack_propagate(False)
        self._border(right)
        self._build_detail_panel(right)

    def _sum_card(self, parent, title, value, color, sym):
        card = tk.Frame(parent, bg=WHITE, padx=20, pady=16)
        card.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 12))
        self._border(card)
        tk.Label(card, text=title, font=(FM, 10), fg=GRAY_400, bg=WHITE).pack(anchor=tk.W)
        tk.Frame(card, height=8, bg=WHITE).pack()
        row = tk.Frame(card, bg=WHITE)
        row.pack(anchor=tk.W)
        tk.Label(row, text=sym, font=(FM, 18), bg=WHITE, fg=color).pack(side=tk.LEFT)
        tk.Label(row, text=f" {value}", font=(FM, 22, "bold"), fg=color, bg=WHITE).pack(side=tk.LEFT)

    def _build_txn_list(self, parent):
        hdr = tk.Frame(parent, bg=WHITE, padx=16, pady=14)
        hdr.pack(fill=tk.X)
        tk.Label(hdr, text="Seznam transakci", font=(FM, 13, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W)
        tk.Label(hdr, text="Vsechny vase prijmy a vydaje za tento mesic",
                 font=(FM, 9), fg=GRAY_400, bg=WHITE).pack(anchor=tk.W)

        sf = tk.Frame(parent, bg=WHITE, padx=16, pady=4)
        sf.pack(fill=tk.X)
        si = tk.Frame(sf, bg=GRAY_100, padx=8, pady=6)
        si.pack(fill=tk.X)
        tk.Label(si, text="🔍", bg=GRAY_100, font=(FM, 10), fg=GRAY_400).pack(side=tk.LEFT)
        tk.Entry(si, textvariable=self.search_var, font=(FM, 10),
                 bg=GRAY_100, fg=GRAY_600, bd=0, relief=tk.FLAT,
                 insertbackground=GRAY_600).pack(side=tk.LEFT, fill=tk.X, expand=True)

        ff = tk.Frame(parent, bg=WHITE, padx=16, pady=6)
        ff.pack(fill=tk.X)
        tk.Label(ff, text="Filtr:", font=(FM, 9), fg=GRAY_600, bg=WHITE).pack(side=tk.LEFT)
        for cat, var in self.cat_filters.items():
            tk.Checkbutton(ff, text=cat, variable=var, font=(FM, 9), bg=WHITE, fg=GRAY_600,
                           activebackground=WHITE, command=self._refresh_table).pack(side=tk.LEFT, padx=3)

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("T.Treeview",
                        background=WHITE, foreground=GRAY_900, fieldbackground=WHITE,
                        rowheight=38, font=(FM, 10))
        style.configure("T.Treeview.Heading",
                        background=GRAY_50, foreground=GRAY_600,
                        font=(FM, 9, "bold"), relief="flat")
        style.map("T.Treeview",
                  background=[("selected", BLUE_LT)],
                  foreground=[("selected", GRAY_900)])

        tf = tk.Frame(parent, bg=WHITE)
        tf.pack(fill=tk.BOTH, expand=True)
        cols = ("date", "desc", "cat", "amount")
        self.tree = ttk.Treeview(tf, columns=cols, show="headings",
                                 style="T.Treeview", selectmode="browse")
        self.tree.heading("date",   text="Datum")
        self.tree.heading("desc",   text="Popis")
        self.tree.heading("cat",    text="Kategorie")
        self.tree.heading("amount", text="Castka")
        self.tree.column("date",   width=110, anchor=tk.W)
        self.tree.column("desc",   width=200, anchor=tk.W)
        self.tree.column("cat",    width=110, anchor=tk.CENTER)
        self.tree.column("amount", width=100, anchor=tk.E)

        vsb = ttk.Scrollbar(tf, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=vsb.set)
        vsb.pack(side=tk.RIGHT, fill=tk.Y)
        self.tree.pack(fill=tk.BOTH, expand=True)
        self.tree.bind("<<TreeviewSelect>>", self._on_row_select)
        self._refresh_table()

    def _refresh_table(self, *_):
        if self.tree is None: return
        for row in self.tree.get_children():
            self.tree.delete(row)
        query  = self.search_var.get().lower()
        active = [c for c, v in self.cat_filters.items() if v.get()]
        for i, t in enumerate(self.transactions):
            if query and query not in t["desc"].lower() and query not in t["cat"].lower():
                continue
            if active and t["cat"] not in active:
                continue
            a = t["amount"]
            s = (f"+{a:,} Kc" if a > 0 else f"{a:,} Kc").replace(",", " ")
            self.tree.insert("", tk.END, iid=str(i),
                             values=(t["date"], t["desc"], t["cat"], s))

    def _build_detail_panel(self, parent):
        tk.Label(parent, text="Detail transakce", font=(FM, 12, "bold"),
                 fg=GRAY_900, bg=WHITE, padx=16, pady=14).pack(anchor=tk.W)
        tk.Label(parent, text="Kliknete na radek v tabulce pro zobrazeni detailu",
                 font=(FM, 9), fg=GRAY_400, bg=WHITE, padx=16, wraplength=240,
                 justify=tk.LEFT).pack(anchor=tk.W)
        tk.Frame(parent, bg=GRAY_200, height=1).pack(fill=tk.X, padx=16, pady=8)
        self.detail_inner = tk.Frame(parent, bg=WHITE)
        self.detail_inner.pack(fill=tk.BOTH, expand=True, padx=16)
        tk.Label(self.detail_inner, text="Vyberte transakci ze seznamu",
                 font=(FM, 10), fg=GRAY_400, bg=WHITE).pack(pady=40)

    def _on_row_select(self, event):
        if self.tree is None: return
        sel = self.tree.selection()
        if not sel: return
        idx = int(sel[0])
        t   = self.transactions[idx]

        for w in self.detail_inner.winfo_children():
            w.destroy()

        a      = t["amount"]
        ac     = GREEN if a > 0 else RED
        as_str = (f"+{a:,} Kc" if a > 0 else f"{a:,} Kc").replace(",", " ")
        bg_c, fg_c = CAT_COLORS.get(t["cat"], (GRAY_100, GRAY_600))

        def row(lbl, val, vc=GRAY_900):
            f = tk.Frame(self.detail_inner, bg=WHITE)
            f.pack(fill=tk.X, pady=3)
            tk.Label(f, text=lbl, font=(FM, 9), fg=GRAY_400, bg=WHITE, width=10, anchor=tk.W).pack(side=tk.LEFT)
            tk.Label(f, text=val, font=(FM, 10, "bold"), fg=vc, bg=WHITE).pack(side=tk.LEFT)

        tk.Frame(self.detail_inner, height=8, bg=WHITE).pack()
        row("Datum:",   t["date"])
        row("Popis:",   t["desc"])
        row("Castka:",  as_str, ac)

        cf = tk.Frame(self.detail_inner, bg=WHITE)
        cf.pack(fill=tk.X, pady=3)
        tk.Label(cf, text="Kategorie:", font=(FM, 9), fg=GRAY_400, bg=WHITE, width=10, anchor=tk.W).pack(side=tk.LEFT)
        tk.Label(cf, text=t["cat"], font=(FM, 9, "bold"), fg=fg_c, bg=bg_c, padx=8, pady=3).pack(side=tk.LEFT)

        tk.Frame(self.detail_inner, bg=GRAY_200, height=1).pack(fill=tk.X, pady=10)
        tk.Button(self.detail_inner, text="🗑  Smazat transakci",
                  font=(FM, 10), fg=RED, bg=RED_LT, bd=0, padx=10, pady=7,
                  cursor="hand2", activebackground="#fecaca",
                  command=lambda i=idx: self._delete_txn(i)).pack(fill=tk.X, pady=3)

    def _delete_txn(self, idx):
        if messagebox.askyesno("Smazat", "Opravdu chcete smazat tuto transakci?"):
            self.transactions.pop(idx)
            self._navigate("overview")

    def _on_search(self, *_):
        self._refresh_table()

    def _show_add_item(self):
        c = self.content
        tk.Label(c, text="Pridat novou transakci", font=(FM, 18, "bold"), fg=GRAY_900, bg=BG).pack(anchor=tk.W)
        tk.Label(c, text=f"Pridani na ucet: {self.current_bank.get()}",
                 font=(FM, 10), fg=GRAY_400, bg=BG).pack(anchor=tk.W, pady=(2, 14))

        form = tk.Frame(c, bg=WHITE, padx=30, pady=24)
        form.pack(fill=tk.X)
        self._border(form)

        tk.Label(form, text="Formular nove transakce", font=(FM, 13, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W)
        tk.Label(form, text="Vyplnte vsechna pole nize.",
                 font=(FM, 9), fg=GRAY_400, bg=WHITE).pack(anchor=tk.W, pady=(2, 16))

        type_var = tk.StringVar(value="expense")
        tg = tk.Frame(form, bg=WHITE)
        tg.pack(fill=tk.X, pady=(0, 14))
        tk.Label(form, text="Typ transakce *", font=(FM, 10, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W)

        btn_exp = tk.Button(tg, text="Vydaj\nExpense", font=(FM, 11, "bold"),
                            fg=RED, bg=RED_LT, bd=1, relief=tk.SOLID,
                            padx=20, pady=10, width=14, cursor="hand2", activebackground=RED_LT)
        btn_inc = tk.Button(tg, text="Prijem\nIncome", font=(FM, 11),
                            fg=GRAY_600, bg=WHITE, bd=1, relief=tk.SOLID,
                            padx=20, pady=10, width=14, cursor="hand2", activebackground=GRAY_50)
        btn_exp.pack(side=tk.LEFT, padx=(0, 8))
        btn_inc.pack(side=tk.LEFT)

        def set_exp():
            type_var.set("expense")
            btn_exp.configure(fg=RED, bg=RED_LT, font=(FM, 11, "bold"))
            btn_inc.configure(fg=GRAY_600, bg=WHITE, font=(FM, 11))
        def set_inc():
            type_var.set("income")
            btn_inc.configure(fg=GREEN, bg=GREEN_LT, font=(FM, 11, "bold"))
            btn_exp.configure(fg=GRAY_600, bg=WHITE, font=(FM, 11))
        btn_exp.configure(command=set_exp)
        btn_inc.configure(command=set_inc)

        def field(lbl, ph=""):
            tk.Label(form, text=lbl, font=(FM, 10, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W, pady=(8, 2))
            e = tk.Entry(form, font=(FM, 11), bg=GRAY_50,
                         fg=GRAY_400 if ph else GRAY_900,
                         bd=1, relief=tk.SOLID, insertbackground=GRAY_900)
            e.pack(fill=tk.X, ipady=7)
            if ph:
                e.insert(0, ph)
                e.bind("<FocusIn>",  lambda ev, ent=e, p=ph:
                       (ent.delete(0, tk.END), ent.configure(fg=GRAY_900)) if ent.get() == p else None)
                e.bind("<FocusOut>", lambda ev, ent=e, p=ph:
                       (ent.insert(0, p), ent.configure(fg=GRAY_400)) if not ent.get() else None)
            return e, ph

        amount_e, amt_ph = field("Castka (Kc) *", "napr. 350")

        tk.Label(form, text="Kategorie *", font=(FM, 10, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W, pady=(8, 2))
        cat_var = tk.StringVar(value="Vyberte kategorii")
        ttk.Combobox(form, textvariable=cat_var, values=CATS,
                     font=(FM, 11), state="readonly").pack(fill=tk.X, ipady=4)

        desc_e, desc_ph = field("Popis *", "napr. Nakup v Albert")

        tk.Label(form, text="Datum *", font=(FM, 10, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W, pady=(8, 2))
        date_e = tk.Entry(form, font=(FM, 11), bg=GRAY_50, fg=GRAY_900,
                          bd=1, relief=tk.SOLID, insertbackground=GRAY_900)
        date_e.insert(0, datetime.now().strftime("%d. %m. %Y"))
        date_e.pack(fill=tk.X, ipady=7)

        btn_row = tk.Frame(form, bg=WHITE)
        btn_row.pack(fill=tk.X, pady=(18, 0))

        def save():
            raw = amount_e.get()
            if raw == amt_ph: raw = ""
            try: amt = float(raw.replace(",", "."))
            except: messagebox.showerror("Chyba", "Zadejte platnou castku"); return
            desc = desc_e.get()
            cat  = cat_var.get()
            date = date_e.get()
            if not desc or desc == desc_ph or cat == "Vyberte kategorii":
                messagebox.showerror("Chyba", "Vyplnte vsechna povinna pole"); return
            if type_var.get() == "expense":
                amt = -abs(amt)
            self.transactions.insert(0, {"date": date, "desc": desc, "cat": cat, "amount": int(amt)})
            messagebox.showinfo("Ulozeno", f"Transakce pridana na ucet {self.current_bank.get()}!")
            self._navigate("overview")

        tk.Button(btn_row, text="💾  Ulozit", font=(FM, 11, "bold"), fg=WHITE,
                  bg=GREEN, bd=0, padx=20, pady=10, cursor="hand2",
                  activebackground="#15803d", command=save).pack(side=tk.LEFT, padx=(0, 10))
        tk.Button(btn_row, text="Zrusit", font=(FM, 11), fg=GRAY_600, bg=GRAY_100,
                  bd=0, padx=20, pady=10, cursor="hand2",
                  activebackground=GRAY_200,
                  command=lambda: self._navigate("overview")).pack(side=tk.LEFT)

    def _show_stats(self):
        c = self.content
        tk.Label(c, text="Statistiky / Statistics", font=(FM, 18, "bold"), fg=GRAY_900, bg=BG).pack(anchor=tk.W)
        tk.Label(c, text=f"Vydaje podle kategorii - {self.current_bank.get()}",
                 font=(FM, 10), fg=GRAY_400, bg=BG).pack(anchor=tk.W, pady=(2, 14))

        cats  = {}
        for t in self.transactions:
            if t["amount"] < 0:
                cats[t["cat"]] = cats.get(t["cat"], 0) + abs(t["amount"])
        total = sum(cats.values()) or 1

        card = tk.Frame(c, bg=WHITE, padx=24, pady=20)
        card.pack(fill=tk.BOTH, expand=True)
        self._border(card)
        tk.Label(card, text="Podil kategorii", font=(FM, 13, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W)
        tk.Label(card, text="Procentualni rozdeleni vydaju",
                 font=(FM, 9), fg=GRAY_400, bg=WHITE).pack(anchor=tk.W, pady=(2, 16))

        cv = tk.Canvas(card, bg=WHITE, highlightthickness=0)
        cv.pack(fill=tk.BOTH, expand=True, pady=8)
        self.update_idletasks()
        w = cv.winfo_width() or 680

        y, bar_h, gap = 10, 28, 14
        for cat, val in sorted(cats.items(), key=lambda x: -x[1]):
            pct   = val / total
            bar_w = max(int(pct * (w - 230)), 4)
            _, fg_c = CAT_COLORS.get(cat, (GRAY_100, GRAY_400))
            cv.create_rectangle(170, y, 170+bar_w, y+bar_h, fill=fg_c, outline="")
            cv.create_text(165, y+bar_h//2, text=cat, anchor=tk.E, font=(FM, 10), fill=GRAY_600)
            cv.create_text(170+bar_w+8, y+bar_h//2,
                           text=f"{val:,} Kc  ({pct*100:.0f}%)".replace(",", " "),
                           anchor=tk.W, font=(FM, 9), fill=GRAY_600)
            y += bar_h + gap
        cv.configure(height=y+10)

    def _show_goals(self):
        c = self.content
        tk.Label(c, text="Cile / Goals", font=(FM, 18, "bold"), fg=GRAY_900, bg=BG).pack(anchor=tk.W)
        tk.Label(c, text="Vase financni cile a plany sporeni",
                 font=(FM, 10), fg=GRAY_400, bg=BG).pack(anchor=tk.W, pady=(2, 14))
        for name, target, saved, color in [
            ("💻 Novy notebook",       12000, 4500,  BLUE),
            ("✈️ Prazdniny v Italii",  25000, 8000,  ORANGE),
            ("📚 Kurzy anglictiny",     5000, 3200,  PURPLE),
        ]:
            card = tk.Frame(c, bg=WHITE, padx=20, pady=16)
            card.pack(fill=tk.X, pady=(0, 12))
            self._border(card)
            top = tk.Frame(card, bg=WHITE)
            top.pack(fill=tk.X)
            tk.Label(top, text=name, font=(FM, 12, "bold"), fg=GRAY_900, bg=WHITE).pack(side=tk.LEFT)
            pct = saved / target
            tk.Label(top, text=f"{pct*100:.0f}%", font=(FM, 11, "bold"), fg=color, bg=WHITE).pack(side=tk.RIGHT)
            pb = tk.Canvas(card, bg=GRAY_100, height=8, highlightthickness=0)
            pb.pack(fill=tk.X, pady=(8, 6))
            card.update_idletasks()
            pw = pb.winfo_width() or 500
            pb.create_rectangle(0, 0, int(pct * pw), 8, fill=color, outline="")
            row = tk.Frame(card, bg=WHITE)
            row.pack(fill=tk.X)
            tk.Label(row, text=f"Nasporeno: {saved:,} Kc".replace(",", " "),
                     font=(FM, 9), fg=GRAY_600, bg=WHITE).pack(side=tk.LEFT)
            tk.Label(row, text=f"Cil: {target:,} Kc".replace(",", " "),
                     font=(FM, 9), fg=GRAY_400, bg=WHITE).pack(side=tk.RIGHT)

    def _show_settings(self):
        c = self.content
        tk.Label(c, text="Nastaveni", font=(FM, 18, "bold"), fg=GRAY_900, bg=BG).pack(anchor=tk.W)
        tk.Label(c, text="Spravujte sve predvolby a nastaveni aplikace",
                 font=(FM, 10), fg=GRAY_400, bg=BG).pack(anchor=tk.W, pady=(2, 14))

        def section(title, sub):
            card = tk.Frame(c, bg=WHITE, padx=24, pady=20)
            card.pack(fill=tk.X, pady=(0, 14))
            self._border(card)
            tk.Label(card, text=title, font=(FM, 12, "bold"), fg=GRAY_900, bg=WHITE).pack(anchor=tk.W)
            tk.Label(card, text=sub,   font=(FM, 9), fg=GRAY_400, bg=WHITE).pack(anchor=tk.W, pady=(2, 12))
            return card

        def lentry(parent, lbl, val):
            tk.Label(parent, text=lbl, font=(FM, 10), fg=GRAY_600, bg=WHITE).pack(anchor=tk.W, pady=(4, 2))
            e = tk.Entry(parent, font=(FM, 11), bg=GRAY_50, fg=GRAY_900,
                         bd=1, relief=tk.SOLID, insertbackground=GRAY_900)
            e.insert(0, val); e.pack(fill=tk.X, ipady=7)

        s1 = section("👤 Osobni informace", "Zakladni informace o vasem uctu")
        lentry(s1, "Jmeno", "Jana Novakova")
        lentry(s1, "E-mail", "jana.novakova@email.cz")

        s2 = section("🌐 Jazyk a mena", "Vyberte preferovany jazyk a menu")
        tk.Label(s2, text="Jazyk aplikace", font=(FM, 10), fg=GRAY_600, bg=WHITE).pack(anchor=tk.W, pady=(4, 2))
        ttk.Combobox(s2, values=["Cestina", "English", "Русский"],
                     font=(FM, 11), state="readonly").pack(fill=tk.X, ipady=4)
        tk.Label(s2, text="Mena", font=(FM, 10), fg=GRAY_600, bg=WHITE).pack(anchor=tk.W, pady=(8, 2))
        ttk.Combobox(s2, values=["Kc (Koruna ceska)", "EUR (Euro)", "USD (Dollar)"],
                     font=(FM, 11), state="readonly").pack(fill=tk.X, ipady=4)

        s3 = section("💲 Mesicni rozpocet", "Nastavte svuj mesicni rozpocet")
        lentry(s3, "Mesicni rozpocet (Kc)", "15000")

        tk.Button(c, text="💾  Ulozit nastaveni", font=(FM, 11, "bold"),
                  fg=WHITE, bg=BLUE, bd=0, padx=20, pady=10, cursor="hand2",
                  activebackground="#1d4ed8",
                  command=lambda: messagebox.showinfo("Ulozeno", "Nastaveni bylo ulozeno!")).pack(anchor=tk.W)

    def _border(self, w):
        w.configure(highlightthickness=1,
                    highlightbackground=GRAY_200,
                    highlightcolor=GRAY_200)


if __name__ == "__main__":
    app = SmartBudgetApp()
    app.mainloop()