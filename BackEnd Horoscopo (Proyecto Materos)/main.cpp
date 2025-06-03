#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h> // Añadido para soporte de caracteres en Windows
#include "C:\Users\marti\OneDrive\Desktop\BackEnd Horoscopo (Proyecto Materos)\swisseph_src/swephexp.h"  // Header principal de Swiss Ephemeris

#ifndef SE_LILITH
#define SE_LILITH 17
#endif

using namespace std;

// Constantes para los signos en español
const string SIGNOS[12] = {
    "Aries", "Tauro", "Géminis", "Cáncer",
    "Leo", "Virgo", "Libra", "Escorpio",
    "Sagitario", "Capricornio", "Acuario", "Piscis"
};

// Convierte longitud eclíptica decimal en "grados-minutos signo"
void formatoSigno(double long_ecl, string &signo, int &grados, int &minutos) {
    if (long_ecl < 0) long_ecl += 360.0;
    long_ecl = fmod(long_ecl, 360.0);
    int idx = int(long_ecl / 30.0);            // 0..11
    double resto = fmod(long_ecl, 30.0);       // [0..30)
    grados = int(resto);
    minutos = int((resto - grados) * 60.0 + 0.5);
    if (minutos == 60) {
        minutos = 0;
        grados += 1;
        if (grados == 30) {
            grados = 0;
            idx = (idx + 1) % 12;
        }
    }
    signo = SIGNOS[idx];
}

struct Ubicacion {
    string pais;
    string provincia;
    double latitud;
    double longitud;
};

// Agrega aquí las ubicaciones que quieras soportar
Ubicacion ubicaciones[] = {
    // Argentina (todas las provincias y CABA)
    {"argentina", "buenos aires", -34.6037, -58.3816},
    {"argentina", "catamarca", -28.4696, -65.7852},
    {"argentina", "chaco", -27.4516, -58.9867},
    {"argentina", "chubut", -43.3002, -65.1023},
    {"argentina", "cordoba", -31.4167, -64.1833},
    {"argentina", "corrientes", -27.4806, -58.8341},
    {"argentina", "entre rios", -31.7319, -60.5238},
    {"argentina", "formosa", -26.1775, -58.1781},
    {"argentina", "jujuy", -24.1858, -65.2995},
    {"argentina", "la pampa", -36.6167, -64.2833},
    {"argentina", "la rioja", -29.4131, -66.8558},
    {"argentina", "mendoza", -32.8908, -68.8272},
    {"argentina", "misiones", -27.3627, -55.9000},
    {"argentina", "neuquen", -38.9516, -68.0591},
    {"argentina", "rio negro", -41.1335, -71.3103},
    {"argentina", "salta", -24.7829, -65.4232},
    {"argentina", "san juan", -31.5375, -68.5364},
    {"argentina", "san luis", -33.2950, -66.3356},
    {"argentina", "santa cruz", -51.6230, -69.2168},
    {"argentina", "santa fe", -31.6333, -60.7000},
    {"argentina", "santiago del estero", -27.7951, -64.2615},
    {"argentina", "tierra del fuego", -54.8019, -68.3029},
    {"argentina", "tucuman", -26.8083, -65.2176},
    {"argentina", "caba", -34.6037, -58.3816}, // Ciudad Autónoma de Buenos Aires

    // España (principales comunidades autónomas y ciudades)
    {"españa", "andalucia", 37.3891, -5.9845},
    {"españa", "aragon", 41.6488, -0.8891},
    {"españa", "asturias", 43.3619, -5.8494},
    {"españa", "baleares", 39.6953, 3.0176},
    {"españa", "canarias", 28.2916, -16.6291},
    {"españa", "cantabria", 43.1828, -3.9878},
    {"españa", "castilla la mancha", 39.8628, -4.0273},
    {"españa", "castilla y leon", 41.6529, -4.7286},
    {"españa", "cataluña", 41.5912, 1.5209},
    {"españa", "extremadura", 39.4937, -6.0679},
    {"españa", "galicia", 42.5751, -8.1339},
    {"españa", "madrid", 40.4168, -3.7038},
    {"españa", "murcia", 37.9922, -1.1307},
    {"españa", "navarra", 42.6954, -1.6761},
    {"españa", "pais vasco", 43.2630, -2.9350},
    {"españa", "la rioja", 42.2871, -2.5396},
    {"españa", "valencia", 39.4699, -0.3763},
    {"españa", "barcelona", 41.3888, 2.1590},
    {"españa", "sevilla", 37.3886, -5.9823},
    {"españa", "zaragoza", 41.6488, -0.8891},

    // México (principales estados)
    {"mexico", "aguascalientes", 21.8853, -102.2916},
    {"mexico", "baja california", 32.5149, -117.0382},
    {"mexico", "baja california sur", 24.1426, -110.3128},
    {"mexico", "campeche", 19.8301, -90.5349},
    {"mexico", "cdmx", 19.4326, -99.1332},
    {"mexico", "chiapas", 16.7569, -93.1292},
    {"mexico", "chihuahua", 28.6353, -106.0889},
    {"mexico", "coahuila", 25.4383, -100.9737},
    {"mexico", "colima", 19.2433, -103.7250},
    {"mexico", "durango", 24.0277, -104.6532},
    {"mexico", "guanajuato", 21.0190, -101.2574},
    {"mexico", "guerrero", 17.5529, -99.5058},
    {"mexico", "hidalgo", 20.1011, -98.7591},
    {"mexico", "jalisco", 20.6597, -103.3496},
    {"mexico", "mexico", 19.4969, -99.7233},
    {"mexico", "michoacan", 19.5665, -101.7068},
    {"mexico", "morelos", 18.6813, -99.1013},
    {"mexico", "nayarit", 21.7514, -104.8455},
    {"mexico", "nuevo leon", 25.6866, -100.3161},
    {"mexico", "oaxaca", 17.0732, -96.7266},
    {"mexico", "puebla", 19.0414, -98.2063},
    {"mexico", "queretaro", 20.5888, -100.3899},
    {"mexico", "quintana roo", 21.1619, -86.8515},
    {"mexico", "san luis potosi", 22.1565, -100.9855},
    {"mexico", "sinaloa", 24.8081, -107.3940},
    {"mexico", "sonora", 29.0729, -110.9559},
    {"mexico", "tabasco", 17.9869, -92.9303},
    {"mexico", "tamaulipas", 23.7369, -99.1411},
    {"mexico", "tlaxcala", 19.3182, -98.2375},
    {"mexico", "veracruz", 19.1738, -96.1342},
    {"mexico", "yucatan", 20.9674, -89.5926},
    {"mexico", "zacatecas", 22.7709, -102.5832}
};
const int num_ubicaciones = sizeof(ubicaciones) / sizeof(Ubicacion);

// Busca latitud y longitud por país y provincia
bool buscarUbicacion(const string& pais, const string& provincia, double& lat, double& lon) {
    for (int i = 0; i < num_ubicaciones; ++i) {
        if (ubicaciones[i].pais == pais && ubicaciones[i].provincia == provincia) {
            lat = ubicaciones[i].latitud;
            lon = ubicaciones[i].longitud;
            return true;
        }
    }
    return false;
}

string explicacionSigno(const string& tipo, const string& signo); // <-- PROTOTIPO

int main() {
    // Configurar consola para UTF-8 en Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Establecer ruta de los efemérides
    swe_set_ephe_path("C:\\Users\\marti\\OneDrive\\Desktop\\BackEnd Horoscopo (Proyecto Materos)\\swisseph_src\\ephe");

    // 1) Pedir datos al usuario
    int dia, mes, anio, hora, minuto;
    double latitud = 0, longitud = 0, zonaHoraria;
    string pais, provincia;
    cout << "País: ";
    getline(cin >> ws, pais);
    cout << "Provincia: ";
    getline(cin, provincia);

    // Convertir a minúsculas para búsqueda robusta
    for (auto &c : pais) c = tolower(c);
    for (auto &c : provincia) c = tolower(c);

    if (buscarUbicacion(pais, provincia, latitud, longitud)) {
        cout << "Latitud: " << latitud << "°, Longitud: " << longitud << "°\n";
    } else {
        cout << "Ubicación no encontrada. Ingresa latitud y longitud manualmente.\n";
        cout << "Latitud (grados, Norte +, Sur -): ";
        cin >> latitud;
        cout << "Longitud (grados, Este +, Oeste -): ";
        cin >> longitud;
        cin.ignore();
    }

    cout << "Fecha nacimiento (dd mm aaaa): ";
    cin >> dia >> mes >> anio;
    cout << "Hora nacimiento (hh mm): ";
    cin >> hora >> minuto;
    cout << "Zona Horaria (horas de diferencia con UTC, ej: -3 para UTC-3): ";
    cin >> zonaHoraria;

    // 2) Convertir la hora local a UT en horas decimales
    double horaLocalDecimal = hora + minuto / 60.0;
    double horaUT = horaLocalDecimal - zonaHoraria;
    // Ajustar para que quede entre 0 y 24
    if (horaUT < 0) horaUT += 24.0;
    if (horaUT >= 24.0) horaUT -= 24.0;

    // 3) Calcular Julian Day para fecha y hora UT
    int flag = SE_GREG_CAL; // calendario gregoriano
    double JD = swe_julday(anio, mes, dia, horaUT, flag);

    // Mostrar Tiempo Universal a usuario
    int hUT_int = int(horaUT);
    int mUT_int = int((horaUT - hUT_int) * 60.0 + 0.5);
    cout << "\nTiempo Universal: "
         << (hUT_int < 10 ? "0" : "") << hUT_int << ":"
         << (mUT_int < 10 ? "0" : "") << mUT_int << "\n";

    // 4) Calcular posición de planetas y puntos
    double xx[6]; // xx[0]=long eclíptica, xx[1]=lat eclíptica, xx[2]=dist rad vectorial
    char serr[256]; // buffer de error

    // Flags: eclíptica geocéntrica (SE_ECL_NUT)
    int pflg = SEFLG_SWIEPH | SEFLG_SPEED;

    // Lista de cuerpos a calcular:
    int cuerpos[] = { SE_SUN, SE_MOON, SE_MERCURY, SE_VENUS, SE_MARS,
                      SE_JUPITER, SE_SATURN, SE_URANUS, SE_NEPTUNE, SE_PLUTO,
                      SE_MEAN_NODE, SE_TRUE_NODE, SE_CHIRON, SE_LILITH };
    string nombres[] = { "Sol", "Luna", "Mercurio", "Venus", "Marte",
                         "Júpiter", "Saturno", "Urano", "Neptuno", "Plutón",
                         "Nodo Norte", "Nodo Sur", "Quirón", "Lilith" };

    int n_cuerpos = sizeof(cuerpos) / sizeof(int);

    // Arrays para almacenar: signo, grados, minutos, casa
    vector<string> signoPlaneta(n_cuerpos);
    vector<int> gPlaneta(n_cuerpos), mPlaneta(n_cuerpos), casaPlaneta(n_cuerpos);

    // 5) Calcular cúspides y Asc/MC
    double cusps[13], ascmc[10];
    // Sistema de casas: 'P' = Plácido. Si quieres otro, pasa 'K' (Koch), 'R' (Regiomontanus), etc.
    char sysCasa = 'P';
    swe_houses(JD, latitud, longitud, sysCasa, cusps, ascmc);
    // ascmc[0] = Ascendente, ascmc[1] = MC (Medio Cielo)

    // 6) Iterar sobre cada planeta/punto y calcular posición
    for (int i = 0; i < n_cuerpos; i++) {
        int nc = cuerpos[i];
        if (swe_calc(JD, nc, pflg, xx, serr) == ERR) {
            cerr << "Error al calcular cuerpo " << nombres[i] << ": " << serr << "\n";
            continue;
        }
        // xx[0] es la longitud eclíptica en grados decimales
        formatoSigno(xx[0], signoPlaneta[i], gPlaneta[i], mPlaneta[i]);
        // Asignar casa
        double long_ecl = xx[0];
        // Normalizo a [0,360)
        if (long_ecl < 0) long_ecl += 360.0;
        while (long_ecl >= 360.0) long_ecl -= 360.0;

        // Encuentro casa usando cúspides
        int c = 12; // fallback
        for (int cidx = 1; cidx <= 12; cidx++) {
            double c1 = cusps[cidx];
            double c2 = (cidx == 12 ? cusps[1] + 360.0 : cusps[cidx+1]);
            double lp = long_ecl;
            if (c2 < c1) c2 += 360.0;
            if (lp < c1) lp += 360.0;
            if (lp >= c1 && lp < c2) {
                c = cidx;
                break;
            }
        }
        casaPlaneta[i] = c;
    }

    // 7) Sacar Ascendente y MC como “planetas” especiales
    string signoAsc; int gAsc, mAsc;
    formatoSigno(ascmc[0], signoAsc, gAsc, mAsc);
    string signoMC; int gMC, mMC;
    formatoSigno(ascmc[1], signoMC, gMC, mMC);

    // 8) Calcular Punto de la Fortuna (F) manualmente
    //    Fortuna (diurna) = ASC + Long(Luna) – Long(Sol)  (si el Sol > 0° eclíptico)
    //    Fortuna (nocturna) = ASC – (Long(Luna) – Long(Sol))
    //    Determinar si es día o noche (el Sol sobre el horizonte). Simplificamos:
    bool esDiurno = false;
    // Si la longitud del Sol está entre Ascendente-90° y Ascendente+90°, consideramos diurno:
    {
        double longSol = 0;
        swe_calc(JD, SE_SUN, pflg, xx, serr);
        longSol = fmod(xx[0] + 360.0, 360.0);
        double ascDeg = fmod(ascmc[0] + 360.0, 360.0);
        double diff = fmod((longSol - ascDeg + 360.0), 360.0);
        if (diff < 180.0) esDiurno = true;
    }
    double longSol, longLun;
    swe_calc(JD, SE_SUN, pflg, xx, serr);   longSol = fmod(xx[0] + 360.0, 360.0);
    swe_calc(JD, SE_MOON, pflg, xx, serr);  longLun = fmod(xx[0] + 360.0, 360.0);

    double longFort;
    if (esDiurno) {
        longFort = ascmc[0] + longLun - longSol;
    } else {
        longFort = ascmc[0] - (longLun - longSol);
    }
    // Normalizo
    while (longFort < 0.0) longFort += 360.0;
    while (longFort >= 360.0) longFort -= 360.0;
    string signoFort; int gFort, mFort;
    formatoSigno(longFort, signoFort, gFort, mFort);

    // Calcular casa de Fortuna
    int casaFort = 12;
    for (int cidx = 1; cidx <= 12; cidx++) {
        double c1 = cusps[cidx];
        double c2 = (cidx == 12 ? cusps[1] + 360.0 : cusps[cidx+1]);
        double lp = longFort;
        if (c2 < c1) c2 += 360.0;
        if (lp < c1) lp += 360.0;
        if (lp >= c1 && lp < c2) {
            casaFort = cidx;
            break;
        }
    }

    // 9) Mostrar resultados en pantalla
    cout << "\n================= ¿QUÉ ES UNA CARTA ASTRAL? =================\n";
    cout << "La carta astral es un mapa del cielo en el momento exacto de tu nacimiento.\n";
    cout << "Muestra la posición del Sol, la Luna y los planetas en los signos y casas,\n";
    cout << "lo que, según la astrología, influye en tu personalidad y tu vida.\n\n";
    cout << "Elementos principales:\n";
    cout << " - Signo Solar: Tu esencia, lo que eres en el fondo.\n";
    cout << " - Signo Lunar: Tu mundo emocional, cómo sientes y reaccionas.\n";
    cout << " - Ascendente: Tu forma de ser ante los demás, tu \"máscara social\".\n";
    cout << " - Medio Cielo: Tu vocación, metas y cómo te ve el mundo profesionalmente.\n";
    cout << " - Fortuna: Un punto que indica oportunidades y bienestar.\n";
    cout << " - Lilith y Quirón: Aspectos profundos de tu personalidad y heridas a sanar.\n";
    cout << " - Casas: Áreas de la vida (familia, trabajo, relaciones, etc.) donde se manifiestan\n";
    cout << "   las energías de los planetas y signos.\n";
    cout << "¡Recuerda! La carta astral es una herramienta de autoconocimiento.\n";
    cout << "No determina tu destino, pero puede ayudarte a comprenderte mejor.\n";
    cout << "--------------------------------------------------------------\n";

    cout << "\n\n================= CARTA ASTRAL =================\n";
    cout << "\nFecha de nacimiento: "
         << setw(2) << setfill('0') << dia << "/"
         << setw(2) << setfill('0') << mes << "/"
         << anio << "\n";
    cout << setfill(' '); // Volver a espacio para texto
    cout << "Lugar: Lat " << latitud << "°, Lon " << longitud << "°\n";
    cout << "Hora local: " << setw(2) << setfill('0') << hora << ":"
         << setw(2) << setfill('0') << minuto << "\n";
    cout << setfill(' ');
    cout << "Tiempo Universal: "
         << setw(2) << setfill('0') << hUT_int << ":"
         << setw(2) << setfill('0') << mUT_int << "\n\n";
    cout << setfill(' ');

    // Signos principales (alineados y comillas prolijas)
    cout << left << setw(14) << "Signo Solar:"   << " " << setw(13) << left << signoPlaneta[0]
         << " (" << right << setw(2) << setfill('0') << gPlaneta[0] << "°"
         << setw(2) << mPlaneta[0] << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Signo Lunar:"   << " " << setw(13) << left << signoPlaneta[1]
         << " (" << right << setw(2) << setfill('0') << gPlaneta[1] << "°"
         << setw(2) << mPlaneta[1] << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Ascendente:"    << " " << setw(13) << left << signoAsc
         << " (" << right << setw(2) << setfill('0') << gAsc << "°"
         << setw(2) << mAsc << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Medio Cielo:"   << " " << setw(13) << left << signoMC
         << " (" << right << setw(2) << setfill('0') << gMC << "°"
         << setw(2) << mMC << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Fortuna:"       << " " << setw(13) << left << signoFort
         << " (" << right << setw(2) << setfill('0') << gFort << "°"
         << setw(2) << mFort << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Lilith:"        << " " << setw(13) << left << signoPlaneta[13]
         << " (" << right << setw(2) << setfill('0') << gPlaneta[13] << "°"
         << setw(2) << mPlaneta[13] << "')\n";
    cout << setfill(' ');
    cout << left << setw(14) << "Quirón:"        << " " << setw(13) << left << signoPlaneta[12]
         << " (" << right << setw(2) << setfill('0') << gPlaneta[12] << "°"
         << setw(2) << mPlaneta[12] << "')\n";
    cout << setfill(' ');

    // Gráfico de columnas “Signo Solar   Signo Lunar   Ascendente” con comillas alineadas
    cout << "\n   Signo Solar      Signo Lunar       Ascendente\n";
    cout << "   " << left << setw(15) << ("\"" + signoPlaneta[0] + "\"")
         << left << setw(17) << ("\"" + signoPlaneta[1] + "\"")
         << left << setw(14) << ("\"" + signoAsc + "\"") << "\n";

    // Separador
    cout << "\n------------------------------------------------------------------\n";
    cout << left << setw(13) << "Planeta"
         << "| " << setw(14) << "Signo"
         << "| " << setw(8) << "Grados"
         << "| " << setw(6) << "Casa" << "\n";
    cout << "------------------------------------------------------------------\n";
    for (int i = 0; i <= 11; i++) { // Solo hasta Nodo Sur (índice 11)
        cout << setfill(' ') << left << setw(13) << nombres[i] << "| "
             << setw(14) << signoPlaneta[i] << "| "
             << right << setw(2) << setfill('0') << gPlaneta[i] << "°"
             << setw(2) << mPlaneta[i] << "'   | "
             << left << setw(6) << setfill(' ') << (casaPlaneta[i] > 0 ? "Casa " + to_string(casaPlaneta[i]) : "—") << "\n";
    }
    // Imprimir Ascendente y MC como recordatorio
    cout << setfill(' ') << left << setw(13) << "Ascendente" << "| "
         << setw(14) << signoAsc << "| "
         << right << setw(2) << setfill('0') << gAsc << "°"
         << setw(2) << mAsc << "'   | "
         << left << setw(6) << setfill(' ') << "Casa 1" << "\n";
    cout << setfill(' ') << left << setw(13) << "Medio Cielo" << "| "
         << setw(14) << signoMC << "| "
         << right << setw(2) << setfill('0') << gMC << "°"
         << setw(2) << mMC << "'   | "
         << left << setw(6) << setfill(' ') << "Casa 10" << "\n";
    cout << setfill(' ') << left << setw(13) << "Fortuna" << "| "
         << setw(14) << signoFort << "| "
         << right << setw(2) << setfill('0') << gFort << "°"
         << setw(2) << mFort << "'   | "
         << left << setw(6) << setfill(' ') << "Casa " << casaFort << "\n";
    cout << setfill(' ') << left << setw(13) << "Lilith" << "| "
         << setw(14) << signoPlaneta[13] << "| "
         << right << setw(2) << setfill('0') << gPlaneta[13] << "°"
         << setw(2) << mPlaneta[13] << "'   | "
         << left << setw(6) << setfill(' ') << "Casa " << casaPlaneta[13] << "\n";
    cout << setfill(' ') << left << setw(13) << "Quirón" << "| "
         << setw(14) << signoPlaneta[12] << "| "
         << right << setw(2) << setfill('0') << gPlaneta[12] << "°"
         << setw(2) << mPlaneta[12] << "'   | "
         << left << setw(6) << setfill(' ') << "Casa " << casaPlaneta[12] << "\n";
    cout << setfill(' ');

    // 10) Finalmente, mostrar el gráfico textual de los 12 signos
    cout << "\n  ##  ARIES      TAURO      GÉMINIS    CÁNCER\n";
    cout << "      LEO        VIRGO      LIBRA      ESCORPIO\n";
    cout << "  ##  SAGITARIO  CAPRICORNIO  ACUARIO  PISCIS\n";

    cout << "\n--- Interpretación personalizada de tus posiciones principales ---\n";
    cout << "Signo Solar en " << signoPlaneta[0] << ": " << explicacionSigno("solar", signoPlaneta[0]) << "\n\n";
    cout << "Signo Lunar en " << signoPlaneta[1] << ": " << explicacionSigno("lunar", signoPlaneta[1]) << "\n\n";
    cout << "Ascendente en " << signoAsc << ": " << explicacionSigno("ascendente", signoAsc) << "\n\n";
    cout << "Medio Cielo en " << signoMC << ": " << explicacionSigno("mc", signoMC) << "\n\n";

    cout << "Mercurio en " << signoPlaneta[2] << ": " << explicacionSigno("mercurio", signoPlaneta[2]) << "\n\n";
    cout << "Venus en " << signoPlaneta[3] << ": " << explicacionSigno("venus", signoPlaneta[3]) << "\n\n";
    cout << "Marte en " << signoPlaneta[4] << ": " << explicacionSigno("marte", signoPlaneta[4]) << "\n\n";
    cout << "Júpiter en " << signoPlaneta[5] << ": " << explicacionSigno("jupiter", signoPlaneta[5]) << "\n\n";
    cout << "Saturno en " << signoPlaneta[6] << ": " << explicacionSigno("saturno", signoPlaneta[6]) << "\n\n";
    cout << "Urano en " << signoPlaneta[7] << ": " << explicacionSigno("urano", signoPlaneta[7]) << "\n\n";
    cout << "Neptuno en " << signoPlaneta[8] << ": " << explicacionSigno("neptuno", signoPlaneta[8]) << "\n\n";
    cout << "Plutón en " << signoPlaneta[9] << ": " << explicacionSigno("pluton", signoPlaneta[9]) << "\n\n";

    cout << "¡Carta Astral generada con éxito! 😉\n";
    return 0;
}

#ifndef SE_LILITH
#define SE_LILITH 17
#endif

string explicacionSigno(const string& tipo, const string& signo) {
    // Signo Solar
    if (tipo == "solar") {
        if (signo == "Aries") return "Eres una persona enérgica, impulsiva y con gran iniciativa. Te gusta liderar y tomar la delantera.";
        if (signo == "Tauro") return "Eres estable, paciente y buscas seguridad. Valoras el confort y la perseverancia.";
        if (signo == "Géminis") return "Eres curioso, comunicativo y versátil. Te adaptas fácilmente y disfrutas aprender.";
        if (signo == "Cáncer") return "Eres sensible, protector y muy familiar. Te guías por tus emociones y cuidas a los tuyos.";
        if (signo == "Leo") return "Eres creativo, generoso y buscas destacar. Tienes un gran corazón y te gusta brillar.";
        if (signo == "Virgo") return "Eres analítico, detallista y servicial. Te gusta ayudar y mejorar lo que te rodea.";
        if (signo == "Libra") return "Eres sociable, diplomático y buscas el equilibrio. Valoras la armonía y las relaciones.";
        if (signo == "Escorpio") return "Eres intenso, apasionado y profundo. Tienes gran capacidad de transformación.";
        if (signo == "Sagitario") return "Eres optimista, aventurero y amante de la libertad. Buscas expandir tus horizontes.";
        if (signo == "Capricornio") return "Eres responsable, ambicioso y perseverante. Te enfocas en tus metas a largo plazo.";
        if (signo == "Acuario") return "Eres original, independiente y humanitario. Te atrae lo diferente y el progreso.";
        if (signo == "Piscis") return "Eres empático, soñador y compasivo. Tienes una gran sensibilidad y creatividad.";
    }
    // Signo Lunar
    if (tipo == "lunar") {
        if (signo == "Aries") return "Emociones impulsivas, reacciones rápidas y necesidad de acción. Te aburres fácilmente y buscas desafíos.";
        if (signo == "Tauro") return "Emociones estables, buscas seguridad y placer. Te reconforta lo material y lo sensorial.";
        if (signo == "Géminis") return "Emociones cambiantes, curiosidad y necesidad de comunicar lo que sientes.";
        if (signo == "Cáncer") return "Emociones profundas, gran sensibilidad y apego al hogar y la familia.";
        if (signo == "Leo") return "Emociones intensas, necesidad de reconocimiento y expresividad emocional.";
        if (signo == "Virgo") return "Emociones analíticas, necesidad de orden y de sentirse útil para los demás.";
        if (signo == "Libra") return "Emociones equilibradas, buscas armonía y relaciones afectivas estables.";
        if (signo == "Escorpio") return "Emociones intensas, pasión y tendencia a la transformación interna.";
        if (signo == "Sagitario") return "Emociones optimistas, necesidad de libertad y de explorar nuevas experiencias.";
        if (signo == "Capricornio") return "Emociones reservadas, autocontrol y necesidad de seguridad material.";
        if (signo == "Acuario") return "Emociones desapegadas, originalidad y necesidad de independencia emocional.";
        if (signo == "Piscis") return "Emociones muy sensibles, empatía y tendencia a soñar despierto.";
    }
    // Ascendente
    if (tipo == "ascendente") {
        if (signo == "Aries") return "Te muestras directo, enérgico y competitivo. Das una imagen de iniciativa y valentía.";
        if (signo == "Tauro") return "Te muestras tranquilo, perseverante y confiable. Das una imagen de estabilidad y sensualidad.";
        if (signo == "Géminis") return "Te muestras comunicativo, curioso y adaptable. Das una imagen juvenil y versátil.";
        if (signo == "Cáncer") return "Te muestras protector, sensible y reservado. Das una imagen de calidez y empatía.";
        if (signo == "Leo") return "Te muestras seguro, creativo y carismático. Das una imagen de liderazgo y generosidad.";
        if (signo == "Virgo") return "Te muestras analítico, modesto y servicial. Das una imagen de orden y pulcritud.";
        if (signo == "Libra") return "Te muestras sociable, diplomático y encantador. Das una imagen de armonía y elegancia.";
        if (signo == "Escorpio") return "Te muestras intenso, misterioso y magnético. Das una imagen de profundidad y poder.";
        if (signo == "Sagitario") return "Te muestras optimista, espontáneo y aventurero. Das una imagen de entusiasmo y libertad.";
        if (signo == "Capricornio") return "Te muestras serio, responsable y reservado. Das una imagen de madurez y ambición.";
        if (signo == "Acuario") return "Te muestras original, independiente y amigable. Das una imagen de innovación y apertura mental.";
        if (signo == "Piscis") return "Te muestras sensible, compasivo y soñador. Das una imagen de empatía y creatividad.";
    }
    // Medio Cielo
    if (tipo == "mc") {
        if (signo == "Aries") return "Vocación hacia el liderazgo, la acción y los desafíos. Buscas destacar por tu iniciativa.";
        if (signo == "Tauro") return "Vocación hacia la estabilidad, lo material y lo artístico. Buscas seguridad y resultados tangibles.";
        if (signo == "Géminis") return "Vocación hacia la comunicación, el aprendizaje y la versatilidad. Destacas en áreas intelectuales.";
        if (signo == "Cáncer") return "Vocación hacia el cuidado, la protección y lo familiar. Destacas en ámbitos de ayuda y contención.";
        if (signo == "Leo") return "Vocación hacia la creatividad, el arte y el liderazgo. Buscas reconocimiento y dejar huella.";
        if (signo == "Virgo") return "Vocación hacia el servicio, la salud y el análisis. Destacas por tu dedicación y perfeccionismo.";
        if (signo == "Libra") return "Vocación hacia la armonía, la justicia y las relaciones. Destacas en trabajos colaborativos.";
        if (signo == "Escorpio") return "Vocación hacia la transformación, la investigación y lo profundo. Destacas en situaciones de crisis.";
        if (signo == "Sagitario") return "Vocación hacia la enseñanza, los viajes y la expansión. Buscas libertad y crecimiento profesional.";
        if (signo == "Capricornio") return "Vocación hacia la autoridad, la estructura y el logro. Buscas éxito y reconocimiento social.";
        if (signo == "Acuario") return "Vocación hacia la innovación, la tecnología y lo social. Destacas por tu originalidad.";
        if (signo == "Piscis") return "Vocación hacia lo artístico, lo espiritual y el servicio. Destacas por tu sensibilidad y empatía.";
    }
    // Mercurio
    if (tipo == "mercurio") {
        if (signo == "Aries") return "Mente rápida, directa y competitiva. Expresas tus ideas con entusiasmo y a veces impulsividad.";
        if (signo == "Tauro") return "Pensamiento práctico, concreto y paciente. Prefieres ideas claras y seguras.";
        if (signo == "Géminis") return "Gran curiosidad, versatilidad y facilidad para comunicarte. Aprendes rápido y te adaptas fácilmente.";
        if (signo == "Cáncer") return "Pensamiento emocional, memoria fuerte y comunicación protectora. Te expresas con sensibilidad.";
        if (signo == "Leo") return "Comunicación creativa, segura y dramática. Te gusta impresionar con tus ideas.";
        if (signo == "Virgo") return "Mente analítica, detallista y lógica. Te expresas de manera precisa y ordenada.";
        if (signo == "Libra") return "Pensamiento equilibrado, diplomático y justo. Buscas armonía en tus palabras.";
        if (signo == "Escorpio") return "Mente profunda, investigadora y perspicaz. Te expresas con intensidad y misterio.";
        if (signo == "Sagitario") return "Pensamiento optimista, filosófico y abierto. Te gusta aprender y compartir conocimientos.";
        if (signo == "Capricornio") return "Mente estructurada, realista y disciplinada. Te comunicas con seriedad y responsabilidad.";
        if (signo == "Acuario") return "Pensamiento original, innovador y a veces excéntrico. Te atraen las ideas nuevas y el futuro.";
        if (signo == "Piscis") return "Mente intuitiva, imaginativa y soñadora. Te comunicas de forma empática y creativa.";
    }
    // Venus
    if (tipo == "venus") {
        if (signo == "Aries") return "Vives el amor con pasión, espontaneidad y entusiasmo. Te atraen los desafíos y la conquista.";
        if (signo == "Tauro") return "Buscas estabilidad, placer y sensualidad en el amor. Eres leal y disfrutas de lo material.";
        if (signo == "Géminis") return "Amor versátil, curioso y comunicativo. Te atraen las personas inteligentes y divertidas.";
        if (signo == "Cáncer") return "Afecto protector, sensible y hogareño. Buscas seguridad emocional y cuidar a quienes amas.";
        if (signo == "Leo") return "Amor generoso, creativo y dramático. Te gusta ser admirado y dar lo mejor de ti.";
        if (signo == "Virgo") return "Amor práctico, servicial y detallista. Demuestras cariño ayudando y cuidando.";
        if (signo == "Libra") return "Amor equilibrado, romántico y sociable. Buscas armonía y belleza en las relaciones.";
        if (signo == "Escorpio") return "Amor intenso, apasionado y transformador. Buscas profundidad y entrega total.";
        if (signo == "Sagitario") return "Amor libre, aventurero y optimista. Te atraen las personas sinceras y espontáneas.";
        if (signo == "Capricornio") return "Amor responsable, serio y comprometido. Buscas relaciones estables y duraderas.";
        if (signo == "Acuario") return "Amor original, independiente y amistoso. Te atraen las relaciones poco convencionales.";
        if (signo == "Piscis") return "Amor compasivo, soñador y desinteresado. Buscas una conexión espiritual y profunda.";
    }
    // Marte
    if (tipo == "marte") {
        if (signo == "Aries") return "Energía directa, iniciativa y valentía. Actúas con rapidez y te gusta liderar.";
        if (signo == "Tauro") return "Energía constante, perseverante y paciente. Actúas con firmeza y buscas resultados tangibles.";
        if (signo == "Géminis") return "Energía mental, versátil y comunicativa. Actúas con ingenio y rapidez.";
        if (signo == "Cáncer") return "Energía protectora, emocional y a veces defensiva. Actúas por instinto y por tus seres queridos.";
        if (signo == "Leo") return "Energía creativa, apasionada y generosa. Actúas con orgullo y buscas reconocimiento.";
        if (signo == "Virgo") return "Energía analítica, detallista y servicial. Actúas con precisión y eficiencia.";
        if (signo == "Libra") return "Energía diplomática, cooperativa y justa. Actúas buscando equilibrio y armonía.";
        if (signo == "Escorpio") return "Energía intensa, apasionada y transformadora. Actúas con determinación y profundidad.";
        if (signo == "Sagitario") return "Energía aventurera, optimista y expansiva. Actúas con entusiasmo y buscas nuevos horizontes.";
        if (signo == "Capricornio") return "Energía disciplinada, ambiciosa y perseverante. Actúas con responsabilidad y metas claras.";
        if (signo == "Acuario") return "Energía innovadora, independiente y original. Actúas de forma impredecible y creativa.";
        if (signo == "Piscis") return "Energía sensible, compasiva y a veces dispersa. Actúas movido por tus sueños y emociones.";
    }
    // Júpiter
    if (tipo == "jupiter") {
        if (signo == "Aries") return "Expansión a través de la iniciativa y el coraje. Confías en ti mismo y buscas nuevas experiencias.";
        if (signo == "Tauro") return "Expansión a través de la estabilidad y los placeres materiales. Valoras la seguridad y la abundancia.";
        if (signo == "Géminis") return "Expansión a través del aprendizaje, la comunicación y la curiosidad. Te adaptas fácilmente.";
        if (signo == "Cáncer") return "Expansión a través de la familia, el hogar y el cuidado. Valoras la protección y la tradición.";
        if (signo == "Leo") return "Expansión a través de la creatividad, el liderazgo y la generosidad. Buscas reconocimiento.";
        if (signo == "Virgo") return "Expansión a través del trabajo, el servicio y la mejora personal. Valoras la precisión y la utilidad.";
        if (signo == "Libra") return "Expansión a través de las relaciones, la justicia y la armonía. Buscas el equilibrio en tu vida.";
        if (signo == "Escorpio") return "Expansión a través de la transformación, la investigación y la intensidad emocional.";
        if (signo == "Sagitario") return "Expansión a través de los viajes, la filosofía y la búsqueda de la verdad. Buscas libertad.";
        if (signo == "Capricornio") return "Expansión a través de la disciplina, la ambición y el logro profesional.";
        if (signo == "Acuario") return "Expansión a través de la innovación, la amistad y las causas sociales. Buscas el progreso.";
        if (signo == "Piscis") return "Expansión a través de la compasión, la espiritualidad y la inspiración artística.";
    }
    // Saturno
    if (tipo == "saturno") {
        if (signo == "Aries") return "Lecciones sobre la paciencia, el autocontrol y el uso constructivo de la energía.";
        if (signo == "Tauro") return "Lecciones sobre el desapego material, la perseverancia y la seguridad interna.";
        if (signo == "Géminis") return "Lecciones sobre la comunicación responsable, la concentración y el pensamiento estructurado.";
        if (signo == "Cáncer") return "Lecciones sobre la gestión emocional, la responsabilidad familiar y el autocuidado.";
        if (signo == "Leo") return "Lecciones sobre el ego, la humildad y el uso responsable del poder personal.";
        if (signo == "Virgo") return "Lecciones sobre la autocrítica, el perfeccionismo y el servicio desinteresado.";
        if (signo == "Libra") return "Lecciones sobre el compromiso, la justicia y las relaciones equilibradas.";
        if (signo == "Escorpio") return "Lecciones sobre el control emocional, la transformación y el desapego.";
        if (signo == "Sagitario") return "Lecciones sobre la responsabilidad en la búsqueda de la verdad y la libertad.";
        if (signo == "Capricornio") return "Lecciones sobre la ambición, la disciplina y la perseverancia.";
        if (signo == "Acuario") return "Lecciones sobre la individualidad, la amistad y el compromiso social.";
        if (signo == "Piscis") return "Lecciones sobre la compasión, los límites y la espiritualidad práctica.";
    }
    // Urano
    if (tipo == "urano") {
        if (signo == "Aries") return "Innovación y cambios repentinos en la forma de actuar. Espíritu pionero y revolucionario.";
        if (signo == "Tauro") return "Cambios inesperados en lo material y en los valores. Búsqueda de libertad financiera.";
        if (signo == "Géminis") return "Ideas originales, mente rápida y comunicación poco convencional.";
        if (signo == "Cáncer") return "Cambios en el hogar y la familia. Emociones poco convencionales.";
        if (signo == "Leo") return "Creatividad original, deseo de destacar de forma única y diferente.";
        if (signo == "Virgo") return "Innovación en el trabajo, la salud y los métodos. Búsqueda de eficiencia diferente.";
        if (signo == "Libra") return "Relaciones poco convencionales, búsqueda de libertad y justicia social.";
        if (signo == "Escorpio") return "Transformaciones profundas, interés por lo oculto y lo revolucionario.";
        if (signo == "Sagitario") return "Ideas progresistas, viajes inesperados y apertura mental.";
        if (signo == "Capricornio") return "Cambios en la carrera, estructuras y ambiciones. Búsqueda de independencia profesional.";
        if (signo == "Acuario") return "Originalidad máxima, espíritu humanitario y visión de futuro.";
        if (signo == "Piscis") return "Intuición revolucionaria, creatividad y sensibilidad fuera de lo común.";
    }
    // Neptuno
    if (tipo == "neptuno") {
        if (signo == "Aries") return "Inspiración para iniciar proyectos creativos y espirituales. Sueños de independencia.";
        if (signo == "Tauro") return "Inspiración artística y sensibilidad hacia lo material y natural.";
        if (signo == "Géminis") return "Imaginación fértil, intuición en la comunicación y tendencia a soñar despierto.";
        if (signo == "Cáncer") return "Gran sensibilidad emocional, intuición y conexión con el inconsciente familiar.";
        if (signo == "Leo") return "Creatividad inspirada, idealismo y deseo de expresar el yo de forma artística.";
        if (signo == "Virgo") return "Inspiración en el servicio, la salud y el trabajo. Sensibilidad a los detalles.";
        if (signo == "Libra") return "Idealismo en las relaciones, búsqueda de armonía y belleza espiritual.";
        if (signo == "Escorpio") return "Intuición profunda, transformación espiritual y atracción por lo oculto.";
        if (signo == "Sagitario") return "Idealismo filosófico, inspiración en los viajes y la espiritualidad.";
        if (signo == "Capricornio") return "Sueños de éxito, inspiración en la carrera y sensibilidad a las estructuras sociales.";
        if (signo == "Acuario") return "Inspiración humanitaria, sueños colectivos y visión de un mundo mejor.";
        if (signo == "Piscis") return "Máxima sensibilidad, empatía y conexión espiritual. Gran creatividad artística.";
    }
    // Plutón
    if (tipo == "pluton") {
        if (signo == "Aries") return "Transformación a través de la acción, la valentía y la autoafirmación.";
        if (signo == "Tauro") return "Transformación profunda en los valores, recursos y seguridad material.";
        if (signo == "Géminis") return "Transformación en la comunicación, el aprendizaje y la mente.";
        if (signo == "Cáncer") return "Transformación emocional, familiar y en el sentido de pertenencia.";
        if (signo == "Leo") return "Transformación en la creatividad, el ego y la autoexpresión.";
        if (signo == "Virgo") return "Transformación en el trabajo, la salud y los hábitos diarios.";
        if (signo == "Libra") return "Transformación en las relaciones, la justicia y la búsqueda de equilibrio.";
        if (signo == "Escorpio") return "Transformación máxima, intensidad emocional y capacidad de renacimiento.";
        if (signo == "Sagitario") return "Transformación en las creencias, la filosofía y la expansión personal.";
        if (signo == "Capricornio") return "Transformación en la carrera, la autoridad y las estructuras sociales.";
        if (signo == "Acuario") return "Transformación en los ideales, la amistad y la visión de futuro.";
        if (signo == "Piscis") return "Transformación espiritual, compasión y conexión con el inconsciente colectivo.";
    }
    return "No hay explicación disponible para este signo.";
}
