// https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%91%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B5_%D0%BF%D1%80%D0%B5%D0%BE%D0%B1%D1%80%D0%B0%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5_%D0%A4%D1%83%D1%80%D1%8C%D0%B5
// AVal - массив анализируемых данных
// FTvl - массив полученных значений
// FFT_SIZE - длина массива должна быть кратна степени 2.

static float sinF[] = {0.0, -1.0, -0.707107, -0.382683, -0.195090, -0.098017, -0.049068, -0.024541, -0.012272, -0.006136};

void FFT(int* AVal, int* FTvl) {
  int i, j, m, Mmax, Istp, count = 0;
  float Tmpr, Tmpi, Tmvl[FFT_SIZE * 2];
  float Wpr, Wr, Wi;

  for (i = 0; i < FFT_SIZE * 2; i += 2) {
    Tmvl[i] = 0;
    Tmvl[i + 1] = AVal[i / 2];
  }

  i = j = 1;
  while (i < FFT_SIZE * 2) {
    if (j > i) {
      Tmpr = Tmvl[i];
      Tmvl[i] = Tmvl[j];
      Tmvl[j] = Tmpr;
      Tmpr = Tmvl[i + 1];
      Tmvl[i + 1] = Tmvl[j + 1];
      Tmvl[j + 1] = Tmpr;
    }
    i = i + 2;
    m = FFT_SIZE;
    while ((m >= 2) && (j > m)) {
      j = j - m;
      m = m >> 1;
    }
    j = j + m;
  }

  Mmax = 2;
  while (FFT_SIZE * 2 > Mmax) {
    Wpr = sinF[count + 1] * sinF[count + 1] * 2;
    Istp = Mmax * 2;
    Wr = 1;
    Wi = 0;
    m = 1;

    while (m < Mmax) {
      i = m;
      m = m + 2;
      Tmpr = Wr;
      Tmpi = Wi;
      Wr += -Tmpr * Wpr - Tmpi * sinF[count];
      Wi += Tmpr * sinF[count] - Tmpi * Wpr;

      while (i < FFT_SIZE * 2) {
        j = i + Mmax;
        Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j - 1];
        Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j - 1];

        Tmvl[j] = Tmvl[i] - Tmpr;
        Tmvl[j - 1] = Tmvl[i - 1] - Tmpi;
        Tmvl[i] = Tmvl[i] + Tmpr;
        Tmvl[i - 1] = Tmvl[i - 1] + Tmpi;
        i = i + Istp;
      }
    }
    count++;
    Mmax = Istp;
  }
  for (i = 0; i < FFT_SIZE; i++) {
    j = i * 2;
    FTvl[i] = (int)(Tmvl[j] * Tmvl[j] + Tmvl[j + 1] * Tmvl[j + 1]) >> 18;
  }
}
